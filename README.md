# Rock Paper Scissors VR
### Using Oculus Hand Tracking. With a little bit of Physics Simulation on top.

## Based on:

- [UHandPoseRecognizer](https://github.com/oculus-samples/Unreal-HandGameplay/blob/main/Plugins/OculusHandTools/Source/OculusHandPoseRecognition/Public/HandPoseRecognizer.h)
- [UPoseableHandComponent](https://github.com/oculus-samples/Unreal-HandGameplay/blob/main/Plugins/OculusHandTools/Source/OculusHandPoseRecognition/Public/PoseableHandComponent.h) (including [this](https://github.com/oculus-samples/Unreal-HandGameplay/pull/11) fix)

## Details:

- [URPS_PoseableHandComponent](Source/RockPaperScissors/Private/Components/RPS_PoseableHandComponent.cpp) is fixing CollisionCapsules in UOculusHandComponent when using OculusHand SkeletalMesh.
- [ARPS_Hand](Source/RockPaperScissors/Private/Hands/RPS_Hand.cpp) can be used inside Pawn as a ChildActor (attached to UMotionControllerComponent). Or can be spawned as is.

### Sample gameplay:

[![Watch the video](https://img.youtube.com/vi/NYaSNUbfiVc/hqdefault.jpg)](https://youtu.be/NYaSNUbfiVc)

### Sample physics:

[![Watch the video](https://img.youtube.com/vi/5ti8ZxKyce8/hqdefault.jpg)](https://youtu.be/5ti8ZxKyce8)
