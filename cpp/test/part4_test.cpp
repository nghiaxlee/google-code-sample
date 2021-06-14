#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../src/videoplayer.h"

using ::testing::HasSubstr;
using ::testing::MatchesRegex;

TEST(Part4, flagVideoWithReason) {
  VideoPlayer videoPlayer = VideoPlayer();
  testing::internal::CaptureStdout();
  videoPlayer.flagVideo("amazing_cats_video_id", "dont_like_cats");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_THAT(
      output,
      HasSubstr(
          "Successfully flagged video: Amazing Cats (reason: dont_like_cats)"));
}

TEST(Part4, flagVideoWithoutReason) {
  VideoPlayer videoPlayer = VideoPlayer();
  testing::internal::CaptureStdout();
  videoPlayer.flagVideo("another_cat_video_id");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_THAT(output, HasSubstr("Successfully flagged video: Another Cat Video "
                                "(reason: Not supplied)"));
}

TEST(Part4, flagVideoAlreadyFlagged) {
  VideoPlayer videoPlayer = VideoPlayer();
  testing::internal::CaptureStdout();
  videoPlayer.flagVideo("amazing_cats_video_id", "dont_like_cats");
  videoPlayer.flagVideo("amazing_cats_video_id", "dont_like_cats");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_THAT(
      output,
      HasSubstr(
          "Successfully flagged video: Amazing Cats (reason: dont_like_cats)"));
  EXPECT_THAT(output, HasSubstr("Cannot flag video: Video is already flagged"));
}

TEST(Part4, flagVideoNonexistent) {
  VideoPlayer videoPlayer = VideoPlayer();
  testing::internal::CaptureStdout();
  videoPlayer.flagVideo("video_does_not_exist", "flagVideo_reason");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_THAT(output, HasSubstr("Cannot flag video: Video does not exist"));
}

TEST(Part4, flagVideoCanNoLongerPlay) {
  VideoPlayer videoPlayer = VideoPlayer();
  testing::internal::CaptureStdout();
  videoPlayer.flagVideo("amazing_cats_video_id");
  videoPlayer.playVideo("amazing_cats_video_id");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_THAT(
      output,
      HasSubstr(
          "Successfully flagged video: Amazing Cats (reason: Not supplied)"));
  EXPECT_THAT(output, HasSubstr("Cannot play video: Video is currently flagged "
                                "(reason: Not supplied)"));
}

TEST(Part4, flagVideoAddVideoToPlaylist) {
  VideoPlayer videoPlayer = VideoPlayer();
  testing::internal::CaptureStdout();
  videoPlayer.flagVideo("amazing_cats_video_id");
  videoPlayer.createPlaylist("my_playlist");
  videoPlayer.addVideoToPlaylist("my_playlist", "amazing_cats_video_id");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_THAT(output,
              HasSubstr("Successfully created new playlist: my_playlist"));
  EXPECT_THAT(output,
              HasSubstr("Cannot add video to my_playlist: Video is "
                        "currently flagged (reason: Not supplied)"));
}

TEST(Part4, flagVideoShowPlaylist) {
  VideoPlayer videoPlayer = VideoPlayer();
  testing::internal::CaptureStdout();
  videoPlayer.createPlaylist("my_playlist");
  videoPlayer.addVideoToPlaylist("my_playlist", "amazing_cats_video_id");
  videoPlayer.flagVideo("amazing_cats_video_id", "dont_like_cats");
  videoPlayer.showPlaylist("my_playlist");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_THAT(output,
              HasSubstr("Successfully created new playlist: my_playlist"));
  EXPECT_THAT(output, HasSubstr("Added video to my_playlist: Amazing Cats"));
  EXPECT_THAT(
      output,
      HasSubstr(
          "Successfully flagged video: Amazing Cats (reason: dont_like_cats)"));
  EXPECT_THAT(output, HasSubstr("Showing playlist: my_playlist"));
  EXPECT_THAT(output, HasSubstr("Amazing Cats (amazing_cats_video_id) [#cat "
                                "#animal] - FLAGGED (reason: dont_like_cats)"));
}

TEST(Part4, flagVideoShowAllVideos) {
  VideoPlayer videoPlayer = VideoPlayer();
  testing::internal::CaptureStdout();
  videoPlayer.flagVideo("amazing_cats_video_id", "dont_like_cats");
  videoPlayer.showAllVideos();
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_THAT(
      output,
      HasSubstr(
          "Successfully flagged video: Amazing Cats (reason: dont_like_cats)"));
  EXPECT_THAT(output, HasSubstr("Here's a list of all available videos:"));
  EXPECT_THAT(output, HasSubstr("Amazing Cats (amazing_cats_video_id) [#cat "
                                "#animal] - FLAGGED (reason: dont_like_cats)"));
  EXPECT_THAT(
      output,
      HasSubstr("Another Cat Video (another_cat_video_id) [#cat #animal]"));
  EXPECT_THAT(output,
              HasSubstr("Funny Dogs (funny_dogs_video_id) [#dog #animal]"));
  EXPECT_THAT(
      output,
      HasSubstr("Life at Google (life_at_google_video_id) [#google #career]"));
}

TEST(Part4, flagVideoSearchVideos) {
  VideoPlayer videoPlayer = VideoPlayer();
  testing::internal::CaptureStdout();
  std::streambuf* orig = std::cin.rdbuf();
  std::istringstream input("no");
  std::cin.rdbuf(input.rdbuf());
  videoPlayer.flagVideo("amazing_cats_video_id", "dont_like_cats");
  videoPlayer.searchVideos("cat");
  std::cin.rdbuf(orig);
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_THAT(
      output,
      HasSubstr(
          "Successfully flagged video: Amazing Cats (reason: dont_like_cats)"));
  EXPECT_THAT(output, HasSubstr("Here are the results for cat:"));
  EXPECT_THAT(output, HasSubstr("1) Another Cat Video (another_cat_video_id) [#cat #animal]"));
  EXPECT_THAT(output, HasSubstr("Would you like to play any of the above? If "
                                "yes, specify the number of the video."));
  EXPECT_THAT(
      output,
      HasSubstr(
          "If your answer is not a valid number, we will assume it's a no."));
  EXPECT_THAT(output, Not(HasSubstr("Amazing Cats (amazing_cats_video_id)")));
}

TEST(Part4, flagVideoStopVideoPlaying) {
  VideoPlayer videoPlayer = VideoPlayer();
  testing::internal::CaptureStdout();
  videoPlayer.playVideo("amazing_cats_video_id");
  videoPlayer.flagVideo("amazing_cats_video_id", "dont_like_cats");
  videoPlayer.showPlaying();
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_THAT(output, HasSubstr("Playing video: Amazing Cats"));
  EXPECT_THAT(
      output,
      HasSubstr(
          "Successfully flagged video: Amazing Cats (reason: dont_like_cats)"));
  EXPECT_THAT(output, HasSubstr("No video is currently playing"));
}

TEST(Part4, allowVideo) {
  VideoPlayer videoPlayer = VideoPlayer();
  testing::internal::CaptureStdout();
  videoPlayer.flagVideo("amazing_cats_video_id", "dont_like_cats");
  videoPlayer.allowVideo("amazing_cats_video_id");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_THAT(
      output,
      HasSubstr(
          "Successfully flagged video: Amazing Cats (reason: dont_like_cats)"));
  EXPECT_THAT(output,
              HasSubstr("Successfully removed flag from video: Amazing Cats"));
}

TEST(Part4, allowVideoNotFlagged) {
  VideoPlayer videoPlayer = VideoPlayer();
  testing::internal::CaptureStdout();
  videoPlayer.allowVideo("amazing_cats_video_id");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_THAT(output,
              HasSubstr("Cannot remove flag from video: Video is not flagged"));
}

TEST(Part4, allowVideoNonexistent) {
  VideoPlayer videoPlayer = VideoPlayer();
  testing::internal::CaptureStdout();
  videoPlayer.allowVideo("video_does_not_exist");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_THAT(output,
              HasSubstr("Cannot remove flag from video: Video does not exist"));
}

TEST(Part4, allowVideoShowPlaylist) {
  VideoPlayer videoPlayer = VideoPlayer();
  testing::internal::CaptureStdout();
  videoPlayer.createPlaylist("my_playlist");
  videoPlayer.addVideoToPlaylist("my_playlist", "amazing_cats_video_id");
  videoPlayer.flagVideo("amazing_cats_video_id", "dont_like_cats");
  videoPlayer.showPlaylist("my_playlist");
  videoPlayer.allowVideo("amazing_cats_video_id");
  videoPlayer.showPlaylist("my_playlist");
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_THAT(output,
              HasSubstr("Successfully created new playlist: my_playlist"));
  EXPECT_THAT(output, HasSubstr("Added video to my_playlist: Amazing Cats"));
  EXPECT_THAT(
      output,
      HasSubstr(
          "Successfully flagged video: Amazing Cats (reason: dont_like_cats)"));
  EXPECT_THAT(output, HasSubstr("Showing playlist: my_playlist"));
  EXPECT_THAT(output, HasSubstr("Amazing Cats (amazing_cats_video_id) [#cat "
                                "#animal] - FLAGGED (reason: dont_like_cats)"));
  EXPECT_THAT(output,
              HasSubstr("Successfully removed flag from video: Amazing Cats"));
  EXPECT_THAT(output,
              HasSubstr("Amazing Cats (amazing_cats_video_id) [#cat #animal]"));
}