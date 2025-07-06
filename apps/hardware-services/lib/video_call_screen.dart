import 'package:flutter/material.dart';
import 'package:stream_video_flutter/stream_video_flutter.dart';

class VideoCallScreen extends StatefulWidget {
  final StreamVideo client;

  const VideoCallScreen({Key? key, required this.client}) : super(key: key);

  @override
  State<VideoCallScreen> createState() => _VideoCallScreenState();
}

class _VideoCallScreenState extends State<VideoCallScreen> {
  late final Call call;

  @override
  void initState() {
    super.initState();
    call = widget.client.makeCall(
      callType: StreamCallType.defaultType(),
      id: 'test-call',
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Video Call'),
      ),
      body: StreamCallContainer(
        call: call,
      ),
    );
  }
}
