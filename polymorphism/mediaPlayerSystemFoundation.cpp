#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>
#include <vector>
using namespace std;

class MediaFile {
protected:
  string filename;
  double duration; // in seconds
  long fileSize;   // in bytes
public:
  MediaFile(const string &name, double dur, long size)
      : filename(name), duration(dur), fileSize(size) {
    cout << "MediaFile created: " << filename << endl;
  }

  // Virtual Destructor
  virtual ~MediaFile() { cout << "MediaFile destroyed: " << filename << endl; }

  // Pure virtual functions - make class abstract
  //
  // making a virtual function to 0 makes this class totally abstract
  // ABSTRACT: if this class has atleast one pure virtual function (i.e.,
  // Abstract), you cannot create objects out of this one
  virtual void play() const = 0;
  virtual void pause() const = 0;
  virtual void stop() const = 0;
  virtual string getMediaType() const = 0;

  // virtual function with default implementation
  virtual void displayInfo() const {
    cout << "File: " << filename << endl;
    cout << "Duration: " << duration << endl;
    cout << "Size: " << fileSize << endl;
    cout << "Type: " << getMediaType() << endl;
  }

  // Non-virtual utility methods
  string getFilename() const { return filename; }
  double getDuration() const { return duration; }
  long getFileSize() const { return fileSize; }
};

// Audio file implementation
class AudioFile : public MediaFile {
private:
  int bitRate;
  string audioFormat;

public:
  AudioFile(const string &name, double dur, long size, int rate,
            const string &format)
      : MediaFile(name, dur, size), bitRate(rate), audioFormat(format) {
    cout << "AudioFile created: " << audioFormat << " format" << endl;
  }
  ~AudioFile() { cout << "Audio File destroyed: " << filename << endl; }

  void play() const override {
    cout << "🎵 Playing audio: " << filename << " (" << audioFormat << ")"
         << endl;
    cout << "BitRate: " << bitRate << " kbps" << endl;
  }

  void pause() const override {
    cout << "⏸️ Audio paused: " << filename << endl;
  }

  void stop() const override {
    cout << "⏹️ Audio stopped: " << filename << endl;
  }

  string getMediaType() const override { return "Audio (" + audioFormat + ")"; }

  // Audio-specific format
  void adjustVolume(int volume) const {
    cout << "🔊 volume set to " << volume << "% for" << filename << endl;
  }

  int getBitRate() const { return bitRate; }

  string getAudioFormat() const { return audioFormat; }
};



// Video file implementation
class VideoFile : public MediaFile {
private:
    string resolution;
    int frameRate;
    string videoCodec;
public:
    VideoFile(const string& name, double dur, long size, const string& res, int fps, const string& codec)
        : MediaFile(name, dur, size), resolution(res), frameRate(fps), videoCodec(codec) {
        cout << "VideoFile created: " << resolution << " @ " << frameRate << "fps" << endl;
    }
    ~VideoFile() {
        cout << "VideoFile destroyed: " << filename << endl;
    }
    void play() const override {
        cout << "🎬 Playing video: " << filename << endl;
        cout << "Resolution: " << resolution << " @ " << frameRate << "fps" << endl;
        cout << "Codec: " << videoCodec << endl;
    }
    void pause() const override {
        cout << "⏸️ Video paused: " << filename << endl;
    }
    void stop() const override {
        cout << "⏹️ Video stopped: " << filename << endl;
    }
    string getMediaType() const override {
        return "Video (" + videoCodec + ")";
    }
    // Video-specific method
    void changeQuality(const string& newRes) const {
        cout << "📺 Quality changed to " << newRes << " for " << filename << endl;
    }
    string getResolution() const { return resolution; }
    int getFrameRate() const { return frameRate; }
    string getVideoCodec() const { return videoCodec; }
};


class MediaPlayerEngine {
private:
  vector<unique_ptr<MediaFile>> playlist;
  int currentTrack;
  bool isPlaying;

public:
  MediaPlayerEngine() : currentTrack(-1), isPlaying(false) {
    cout << "MediaPlayerEngine initialized" << endl;
  }
  ~MediaPlayerEngine() { cout << "MediaPlayerEngine shutting down" << endl; }
  // Add media to playlist
  void addMedia(unique_ptr<MediaFile> media) {
    cout << "Added to playlist: " << media->getFilename() << endl;
    playlist.push_back(std::move(media));
  }
  // Polymorphic playback - works with any mediaFile type
  void playAll() const {
    cout << "\n Playing entire playlist: " << endl;
    for (const auto &media : playlist) {
      cout << "\n--- Now Playing ---" << endl;
      media->displayInfo(); // virtual call
      media->play();        // virtual call - dynamic binding
      cout << "---------------------" << endl;
    }
  }

  // RTTI demonstration with type-specific behavior
  void playWithEnhancements(int index) const {
    if (index < 0 || index >= playlist.size()) {
      cout << "Invalid track index!" << endl;
      return;
    }
    MediaFile *media = playlist[index].get();
    cout << "\n🚀  Enhanced playback with RTTI:" << endl;
    media->displayInfo();
    media->play();
    // use RTTI for type-specific enhancements
    if (auto *audio = dynamic_cast<AudioFile *>(media)) {
      cout << "Audio specific enhancements: " << endl;
      cout << "  Type detected: " << typeid(*audio).name() << endl;
      audio->adjustVolume(75);
      if (audio->getAudioFormat() == "MP3") {
        cout << "   Applied MP3 equalizer preset" << endl;
      }
    } else if (auto *video = dynamic_cast<VideoFile *>(media)) {
      cout << "Video-specific enhancements: " << endl;
      cout << " Type detected: " << typeid(*video).name() << endl;
      video->changeQuality("1080p");
      if (video->getFrameRate() > 30) {
        cout << "   Enabled smooth motion interpolation" << endl;
      }
    } else {
        cout << "Unknown media type: " << typeid(*media).name() << endl;
    }
  }

  // Demonstrate virtual function table concepts
  void analyzeVirtualDispatch() const {
    cout << "\n Virtual Function Analysis:" << endl;
    for (size_t i = 0; i < playlist.size(); ++i) {
      MediaFile *media = playlist[i].get();
      cout << "\nTrack " << (i + 1) << ":" << endl;
      cout << " Object address: " << media << endl;
      cout << " TypeID: " << typeid(*media).name() << endl;
      cout << " Virtual dispatch test: " << endl;
      // These calls go through vtable lookup
      cout << "     getMediaType(): " << media->getMediaType() << endl;
      cout << "     Polumorphic behavior confirmed ✔️" << endl;
    }
  }

  // Get playlist statistics
  void showPlaylistStats() const {
    cout << "\n Playlist statistics: " << endl;
    cout << "Total files: " << playlist.size() << endl;
    int audioCount = 0, videoCount = 0;
    double totalDuration = 0;
    long totalSize = 0;
    for (const auto &media : playlist) {
      totalDuration += media->getDuration();
      totalSize += media->getFileSize();
      // Count by type using RAII
      if (dynamic_cast<AudioFile *>(media.get())) {
          audioCount++;
      } else if (dynamic_cast<VideoFile *>(media.get())) {
          videoCount++;
      }
    }
    cout << "Audio files: " << audioCount << endl;
    cout << "Video files: " << videoCount << endl;
    cout << "Total duration: " << totalDuration << " seconds" << endl;
    cout << "Total size: " << totalSize << " bytes" << endl;
  }
  size_t getPlaylistSize() const { return playlist.size(); }
};

// Utility function for media player operations
void demonstrastePolymorphicBehavior(
    const vector<unique_ptr<MediaFile>> &mediaFiles) {
  cout << "\n🎭 Polymorphic Behavior Demonstration:" << endl;
  for (const auto &media : mediaFiles) {
    cout << "\n" << string(40, '-') << endl;
    // Polymorphic calls - same interface, different behavior
    media->displayInfo();
    media->play();
    media->pause();
    media->stop();
    cout << string(40, '-') << endl;
  }
}
