#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <regex>

class UserProfile {
private:
  std::string username;
  std::string email;
  std::string passwordHash;
  std::string fullName;
  int age;
  std::vector<std::string> friends;
  bool isPrivateProfile;
  bool isVerified;

  // Private validation helpers
  bool isValidEmail(const std::string &email) const {
    std::regex emailPattern(
        R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email, emailPattern);
  }
  bool isValidUsername(const std::string &username) const {
      return username.length() >= 3 && username.length() <= 20;
  }

public:
  // ctr
  UserProfile(const std::string &user, const std::string &mailstring,
              const std::string &name)
      : isPrivateProfile(true), isVerified(false), age(0) {
    if (!setUsername(user)) {
      throw std::invalid_argument("Invalid username");
    }

    if (!setEmail(mailstring)) {
       throw std::invalid_argument("Invalid email address");
     }
    if (!setFullName(name)) {
       throw std::invalid_argument("Invalid Full name");
     }
 }

  // getters for public information
  std::string getUsername() const { return username; }
  std::string getEmail() const { return email; }
  std::string getDisplayName() const { return isPrivateProfile ? username : fullName; }
  bool isProfilePrivate() const { return isPrivateProfile; }
  bool isUserVerified() const { return isVerified; }
  int getFriendCount() const { return friends.size(); }

  // Secured setters with validation
  bool setUsername(const std::string &newUsername) {
    if (isValidUsername(newUsername)) {
      username = newUsername;
      return true;
    }
    std::cout << "Error: Username must be 3-20 chars long" << std::endl;
    return false;
  }

  bool setEmail(const std::string &newEmail) {
    if (isValidEmail(newEmail)) {
      email = newEmail;
      return true;
    }
    std::cout << "Error: Invalid email format" << std::endl;
    return false;
  }

  bool setFullName(const std::string &name) {
    if (!name.empty() && name.length() <= 100) {
        fullName = name;
        return true;
    }
    std::cout << "Error: Full name cannot be empty or exceed 100 chars"
              << std::endl;
    return false;
  }

  bool setAge(int newAge) {
    if (newAge >= 13 && newAge <= 120) {
      age = newAge;
      return true;
    }
    std::cout << "Error: Age must be between 13 and 120" << std::endl;
    return false;
  }


  void setPrivacyStatus() {
    isPrivateProfile = true;
    std::cout << "making the profile(" << username << ") private"   << std::endl;
  }

  void togglePrivacy(bool status) {
    isPrivateProfile = status;
    std::cout << "setting the profile(" << username << ") to " << (isPrivateProfile ? "private" : "public") << std::endl;
  }

  void addFriend(const std::string& name) {
    if (isValidUsername(name)) {
      friends.push_back(name);
      std::cout << "Added " << name << " to " << username << "'s friends list";
    } else {
      std::cout << "friend name " << name << " is invalid\n";
    }
  }

  void removeFriend(const std::string& name) {
    auto it = std::find(friends.begin(), friends.end(), name);
    if (it != friends.end()) {
        friends.erase(it);
    } else {
      std::cout  << name << " doesn't exist in " << username << "'s friend list" << std::endl;
    }
  }

  friend class PlatformModerator;
  friend void technicalSupport(UserProfile& user, const std::string& issue);
};


void technicalSupport(UserProfile &profile, const std::string& issue) {
  std::cout << "\n === TECH SUPPORT ENABLED ===" << std::endl;
  std::cout << "Issue: " << issue << std::endl;
  std::cout << "Profile Diagnosis...\n";
        std::cout << "username: " << profile.username << std::endl;
        std::cout << "age: " << profile.age << std::endl;
        std::cout << "fullname: " << profile.fullName << std::endl;

        std::cout << "friends: [";
        for (const auto &f : profile.friends) {
            std::cout << f << ", ";
        }
        std::cout << "]\n";
}

class PlatformModerator {
public:
  void performModeration(const UserProfile &profile) {
    std::cout << "\n === PROFILE MODERATION ===" << std::endl;
    std::cout << "username: " << profile.username << std::endl;
    std::cout << "age: " << profile.age << std::endl;
    std::cout << "fullname: " << profile.fullName << std::endl;

    std::cout << "friends: [";
    for (const auto &f : profile.friends) {
        std::cout << f << ", ";
    }
    std::cout << "]\n";
 }
};




int main() {

    UserProfile u1("akashas", "akash@gmail.com", "Akash Sajja" );
    UserProfile u2("balu", "balu@gmail.com", "balu bot");
    u1.setAge(20);
    u2.setAge(21);

    std::cout  << u2.getUsername() << std::endl;
    std::cout << u1.getUsername() << std::endl;

    u1.addFriend(u2.getUsername());
    u2.addFriend(u1.getUsername());

  //  u2.removeFriend(u1.getUsername());
//    u2.removeFriend("babji");


    std::cout << "set full names\n";

    std::cout << "Set to public for u1\n";
    u2.togglePrivacy(false);
    u1.togglePrivacy(false);
    u1.setPrivacyStatus();
    u2.togglePrivacy(true);


    std::cout <<"\n testing tech support\n";
    technicalSupport(u1, "suss account!");


    std::cout << "\n platform moderation: \n";
    PlatformModerator p;
    p.performModeration(u2);
    p.performModeration(u1);



  return 0;
}
