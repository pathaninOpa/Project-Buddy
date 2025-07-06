import 'dart:async';
import 'package:flutter/material.dart';

void main() => runApp(ChatBotFaceApp());

class ChatBotFaceApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: ChatBotFaceScreen(),
      debugShowCheckedModeBanner: false,
    );
  }
}

class ChatBotFaceScreen extends StatefulWidget {
  @override
  _ChatBotFaceScreenState createState() => _ChatBotFaceScreenState();
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
          // Input field
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
                      hintText: 'Say something...',
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
              ],
            ),
          ),
        ],
      ),
    );
  }
}
