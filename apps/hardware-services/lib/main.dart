
import 'dart:async';
import 'package:flutter/material.dart';
import 'package:stream_video_flutter/stream_video_flutter.dart';

void main() {
  final client = StreamVideo(
    'btCkosupZ1Vq', // Replace with your actual API key
    user: User.regular(userId: 'IG_88', role: 'admin', name: 'Suttikarn'),
    userToken: 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJodHRwczovL3Byb250by5nZXRzdHJlYW0uaW8iLCJzdWIiOiJ1c2VyL0lHXzg4IiwidXNlcl9pZCI6IklHXzg4IiwidmFsaWRpdHlfaW5fc2Vjb25kcyI6NjA0ODAwLCJpYXQiOjE3NTE0NTUxNDAsImV4cCI6MTc1MjA1OTk0MH0.XOkgQ5q2LJw-3TR6dK4GMXBSQf_R2ZW1bmYy0GArIxs', // Replace with your user token
  );

  runApp(ChatBotFaceApp(client: client));
}

class ChatBotFaceApp extends StatelessWidget {
  final StreamVideo client;

  const ChatBotFaceApp({super.key, required this.client});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: ChatBotFaceScreen(client: client),
      debugShowCheckedModeBanner: false,
    );
  }
}

class ChatBotFaceScreen extends StatefulWidget {
  final StreamVideo client;

  const ChatBotFaceScreen({super.key, required this.client});

  @override
  State<ChatBotFaceScreen> createState() => _ChatBotFaceScreenState();
}

class _ChatBotFaceScreenState extends State<ChatBotFaceScreen>
    with TickerProviderStateMixin {
  final TextEditingController _controller = TextEditingController();
  bool _isTalking = false;
  bool _eyesClosed = false;
  bool _mouthOpen = false;

  Timer? _blinkTimer;
  Timer? _talkingMouthTimer;

  @override
  void initState() {
    super.initState();
    _startBlinking();
  }

  void _startBlinking() {
    _blinkTimer = Timer.periodic(Duration(seconds: 4), (_) async {
      setState(() => _eyesClosed = true);
      await Future.delayed(Duration(milliseconds: 200));
      if (mounted) setState(() => _eyesClosed = false);
    });
  }

  void _onSendMessage() {
    final input = _controller.text.trim();
    if (input.isEmpty) return;

    final durationMs = (input.length * 60).clamp(600, 5000);

    setState(() {
      _isTalking = true;
    });

    _talkingMouthTimer = Timer.periodic(Duration(milliseconds: 300), (_) {
      setState(() {
        _mouthOpen = !_mouthOpen;
      });
    });

    Future.delayed(Duration(milliseconds: durationMs), () {
      _talkingMouthTimer?.cancel();
      if (mounted) {
        setState(() {
          _isTalking = false;
          _mouthOpen = false;
        });
      }
    });

    _controller.clear();
  }

  @override
  void dispose() {
    _blinkTimer?.cancel();
    _talkingMouthTimer?.cancel();
    _controller.dispose();
    super.dispose();
  }

  Widget _buildEye(bool isClosed) {
    return AnimatedContainer(
      duration: Duration(milliseconds: 100),
      width: 30,
      height: isClosed ? 4 : 30,
      decoration: BoxDecoration(
        color: Colors.black,
        borderRadius: BorderRadius.circular(15),
      ),
    );
  }

  Widget _buildMouth() {
    if (_isTalking && _mouthOpen) {
      return Container(
        width: 50,
        height: 50,
        decoration: BoxDecoration(
          color: Colors.black,
          shape: BoxShape.circle,
        ),
      );
    } else {
      return Container(
        width: 90,
        height: 45,
        decoration: BoxDecoration(
          border: Border(
            bottom: BorderSide(width: 6, color: Colors.black),
          ),
          borderRadius: BorderRadius.vertical(bottom: Radius.circular(90)),
        ),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    final size = MediaQuery.of(context).size;

    return Scaffold(
      backgroundColor: Colors.black,
      body: Stack(
        children: [
          // Fullscreen face background
          Container(
            width: size.width,
            height: size.height,
            decoration: BoxDecoration(
              color: Colors.yellow.shade300,
              shape: BoxShape.rectangle,
            ),
          ),
          // Facial features
          Positioned(
            top: size.height * 0.25,
            left: size.width * 0.25 - 15,
            child: _buildEye(_eyesClosed),
          ),
          Positioned(
            top: size.height * 0.25,
            right: size.width * 0.25 - 15,
            child: _buildEye(_eyesClosed),
          ),
          Positioned(
            top: size.height * 0.55,
            left: (size.width / 2) - 45,
            child: _buildMouth(),
          ),
          // Input + video call button
          Positioned(
            bottom: 20,
            left: 20,
            right: 20,
            child: Row(
              children: [
                Expanded(
                  child: TextField(
                    controller: _controller,
                    decoration: InputDecoration(
                      hintText: 'พิมพ์ข้อความ...',
                      fillColor: Colors.white,
                      filled: true,
                      border: OutlineInputBorder(),
                    ),
                  ),
                ),
                SizedBox(width: 10),
                ElevatedButton(
                  onPressed: _onSendMessage,
                  child: Text("Send"),
                ),
                SizedBox(width: 10),
                ElevatedButton(
                  onPressed: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (context) =>
                            VideoCallScreen(client: widget.client),
                      ),
                    );
                  },
                  child: Icon(Icons.video_call),
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}

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
      appBar: AppBar(title: Text('Video Call')),
      body: StreamCallContainer(call: call),
    );
  }
}
