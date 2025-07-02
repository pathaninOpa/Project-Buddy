import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_webrtc/flutter_webrtc.dart';
import 'package:flutterapp01/userdata.dart'; // Your CareGiver class
import 'dart:convert';
import 'package:web_socket_channel/io.dart';

void main() {
  SystemChrome.setSystemUIOverlayStyle(
    const SystemUiOverlayStyle(statusBarColor: Colors.transparent),
  );
  runApp(const MaterialApp(
    debugShowCheckedModeBanner: false,
    home: BuddyHomePage(),
  ));
}

class BuddyHomePage extends StatefulWidget {
  const BuddyHomePage({super.key});

  @override
  State<BuddyHomePage> createState() => _BuddyHomePageState();
}

class _BuddyHomePageState extends State<BuddyHomePage> {
  List<Buddy> buddies = [
    Buddy(name: 'Buddy 1', imagePath: 'assets/Buddy1.jpeg'),
  ];

  void _addBuddy() {
    setState(() {
      buddies.add(Buddy(
        name: 'Buddy ${buddies.length + 1}',
        imagePath: 'assets/Buddy1.jpeg',
      ));
    });
  }

  @override
  Widget build(BuildContext context) {
    final caregiver = CareGiver(
      cgname: 'Jane Doe',
      cgage: '35',
      cggender: 'Female',
      cgrole: 'Mother',
    );

    return Scaffold(
      resizeToAvoidBottomInset: true,
      backgroundColor: const Color.fromRGBO(22, 45, 65, 1),
      appBar: AppBar(
        backgroundColor: Colors.transparent,
        elevation: 0,
        automaticallyImplyLeading: false,
        titleSpacing: 0,
        title: Row(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const SizedBox(width: 20),
            const Icon(Icons.menu, color: Colors.white, size: 30),
            const SizedBox(width: 10),
            Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                const Text(
                  'Welcome,',
                  style: TextStyle(
                    color: Color(0xFFFFAFA0),
                    fontSize: 22,
                    fontWeight: FontWeight.w600,
                  ),
                ),
                Text(
                  caregiver.cgname,
                  style: const TextStyle(
                    color: Colors.white,
                    fontSize: 26,
                    fontWeight: FontWeight.bold,
                  ),
                ),
              ],
            ),
            const Spacer(),
            const Padding(
              padding: EdgeInsets.only(right: 16),
              child: CircleAvatar(
                radius: 30,
                backgroundImage: AssetImage('assets/profile.jpeg'),
                backgroundColor: Colors.transparent,
              ),
            ),
          ],
        ),
      ),
      body: Padding(
        padding: const EdgeInsets.fromLTRB(20, 60, 16, 16),
        child: GridView.count(
          crossAxisCount: 2,
          crossAxisSpacing: 20.0,
          mainAxisSpacing: 40,
          children: [
            ...buddies.map((buddy) {
              return GestureDetector(
                onTap: () async {
                  final updatedBuddy = await Navigator.push<Buddy>(
                    context,
                    MaterialPageRoute(
                      builder: (context) => BuddyDetailPage(buddy: buddy),
                    ),
                  );
                  if (updatedBuddy != null) {
                    setState(() {
                      final index = buddies.indexOf(buddy);
                      buddies[index] = updatedBuddy;
                    });
                  }
                },
                child: Column(
                  mainAxisSize: MainAxisSize.min,
                  children: [
                    Container(
                      width: 150,
                      height: 150,
                      child: ClipRRect(
                        borderRadius: BorderRadius.circular(16),
                        child: Image.asset(
                          buddy.imagePath,
                          fit: BoxFit.cover,
                        ),
                      ),
                    ),
                    const SizedBox(height: 10),
                    Text(
                      buddy.name,
                      style: const TextStyle(
                        color: Colors.white,
                        fontSize: 14,
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                  ],
                ),
              );
            }).toList(),
            GestureDetector(
              onTap: _addBuddy,
              child: Column(
                mainAxisSize: MainAxisSize.min,
                children: [
                  //AspectRatio(
                    //aspectRatio: 1,
                     Container(
                      width: 150,
                      height: 150,
                      decoration: BoxDecoration(
                        color: const Color(0xFF446178),
                        borderRadius: BorderRadius.circular(7),
                        border: Border.all(color: Colors.white38, width: 2),
                      ),
                      child: const Center(
                        child: Icon(Icons.add, color: Colors.white, size: 32),
                      ),
                    ),
                  //),
                  const SizedBox(height: 10),
                  const Text(
                    'Add',
                    style: TextStyle(
                      color: Colors.white70,
                      fontSize: 14,
                      fontWeight: FontWeight.w500,
                    ),
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}

class BuddyDetailPage extends StatefulWidget {
  final Buddy buddy;

  const BuddyDetailPage({super.key, required this.buddy});

  @override
  State<BuddyDetailPage> createState() => _BuddyDetailPageState();
}

class _BuddyDetailPageState extends State<BuddyDetailPage> {
  late TextEditingController _nameController;

  @override
  void initState() {
    super.initState();
    _nameController = TextEditingController(text: widget.buddy.name);
  }

  @override
  void dispose() {
    _nameController.dispose();
    super.dispose();
  }

  void _saveChanges() {
    Navigator.pop(
      context,
      Buddy(
        name: _nameController.text,
        imagePath: widget.buddy.imagePath,
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0xFF162D41),
      appBar: AppBar(
        title: const Text('Elder Profile',
                      style: TextStyle(color: Colors.white)),
        backgroundColor: const Color.fromARGB(0, 239, 235, 235),
        elevation: 0,
        actions: [
          IconButton(
            icon: const Icon(Icons.save),
            onPressed: _saveChanges,
            tooltip: 'Save Buddy Name',
          ),
        ],
      ),
      body: Padding(
        padding: const EdgeInsets.all(24),
        child: Column(
          children: [
            CircleAvatar(
              radius: 60,
              backgroundImage: AssetImage(widget.buddy.imagePath),
            ),
            const SizedBox(height: 16),
            TextField(
              controller: _nameController,
              style: const TextStyle(color: Colors.white),
              decoration: const InputDecoration(
                labelText: 'Buddy Name',
                labelStyle: TextStyle(color: Colors.white70),
                enabledBorder: UnderlineInputBorder(
                  borderSide: BorderSide(color: Colors.white24),
                ),
              ),
            ),
            const SizedBox(height: 24),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: [
                _actionButton(
                  icon: Icons.video_call,
                  label: 'Video Call',
                  color: Colors.green,
                  onTap: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (_) => const VideoCallPage()),
                    );
                  },
                ),
                _actionButton(
                  icon: Icons.notifications,
                  label: 'Reminder',
                  color: Colors.blueAccent,
                  onTap: () {},
                ),
                _actionButton(
                  icon: Icons.health_and_safety,
                  label: 'Health',
                  color: Colors.pinkAccent,
                  onTap: () {},
                ),
              ],
            ),
            const SizedBox(height: 32),
            Container(
              padding: const EdgeInsets.all(16),
              decoration: BoxDecoration(
                color: const Color(0xFF2C4A60),
                borderRadius: BorderRadius.circular(12),
              ),
              child: const Text(
                'This is a sample summary. Add notes or reminders here to help you manage care effectively.',
                style: TextStyle(
                  color: Colors.white70,
                  fontSize: 15,
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }

  Widget _actionButton({
    required IconData icon,
    required String label,
    required Color color,
    required VoidCallback onTap,
  }) {
    return Column(
      children: [
        Ink(
          decoration: ShapeDecoration(
            color: color,
            shape: const CircleBorder(),
          ),
          child: IconButton(
            icon: Icon(icon, color: Colors.white),
            iconSize: 32,
            onPressed: onTap,
          ),
        ),
        const SizedBox(height: 6),
        Text(
          label,
          style: const TextStyle(color: Colors.white70, fontSize: 14),
        ),
      ],
    );
  }
}

class VideoCallPage extends StatefulWidget {
  const VideoCallPage({super.key});

  @override
  State<VideoCallPage> createState() => _VideoCallPageState();
}

class _VideoCallPageState extends State<VideoCallPage> {
  final RTCVideoRenderer _localRenderer = RTCVideoRenderer();
  final RTCVideoRenderer _remoteRenderer = RTCVideoRenderer();
  RTCPeerConnection? _peerConnection;
  MediaStream? _localStream;

  IOWebSocketChannel? _channel;

  bool _inCall = false;

  @override
  void initState() {
    super.initState();
    _initRenderers();
    _connectToSignalingServer();
  }

  @override
  void dispose() {
    _hangUp();
    _localRenderer.dispose();
    _remoteRenderer.dispose();
    super.dispose();
  }

  Future<void> _initRenderers() async {
    await _localRenderer.initialize();
    await _remoteRenderer.initialize();
    await _startLocalCamera();
  }

  Future<void> _startLocalCamera() async {
    final stream = await navigator.mediaDevices.getUserMedia({
      'audio': true,
      'video': {'facingMode': 'user'}
    });
    _localRenderer.srcObject = stream;
    _localStream = stream;
  }

  void _connectToSignalingServer() {
    _channel = IOWebSocketChannel.connect('ws://192.168.1.5:8080'); // << replace with your local IP

    _channel!.stream.listen((message) async {
      final decoded = jsonDecode(message);
      final event = decoded['event'];
      final data = decoded['data'];

      switch (event) {
        case 'answer':
          await _peerConnection?.setRemoteDescription(
            RTCSessionDescription(data['sdp'], data['type']),
          );
          break;
        case 'ice_candidate':
          final candidate = RTCIceCandidate(
            data['candidate'],
            data['sdpMid'],
            data['sdpMLineIndex'],
          );
          await _peerConnection?.addCandidate(candidate);
          break;
      }
    });
  }

  Future<void> _createPeerConnection() async {
    final config = {
      'iceServers': [
        {'urls': 'stun:stun.l.google.com:19302'},
      ]
    };

    _peerConnection = await createPeerConnection(config);

    _localStream?.getTracks().forEach((track) {
      _peerConnection!.addTrack(track, _localStream!);
    });

    _peerConnection!.onTrack = (event) {
      if (event.streams.isNotEmpty) {
        _remoteRenderer.srcObject = event.streams[0];
      }
    };

    _peerConnection!.onIceCandidate = (candidate) {
      _channel!.sink.add(jsonEncode({
        'event': 'ice_candidate',
        'data': candidate.toMap(),
      }));
    };
  }

  Future<void> _callElder() async {
    setState(() => _inCall = true);

    await _createPeerConnection();
    final offer = await _peerConnection!.createOffer();
    await _peerConnection!.setLocalDescription(offer);

    _channel!.sink.add(jsonEncode({
      'event': 'offer',
      'data': {
        'sdp': offer.sdp,
        'type': offer.type,
      },
    }));
  }

  Future<void> _hangUp() async {
    setState(() => _inCall = false);

    try {
      _remoteRenderer.srcObject = null;
      _localRenderer.srcObject = null;

      await _peerConnection?.close();
      _peerConnection = null;

      _localStream?.getTracks().forEach((track) => track.stop());
      _localStream = null;

      await _startLocalCamera(); // restart camera for preview
    } catch (e) {
      print('Error while hanging up: $e');
    }
  }

  Widget _buildControlButtons() {
    return Row(
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        ElevatedButton.icon(
          onPressed: _inCall ? null : _callElder,
          icon: const Icon(Icons.call),
          label: const Text("Call Elder"),
          style: ElevatedButton.styleFrom(
            backgroundColor: Colors.green,
            padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 12),
          ),
        ),
        const SizedBox(width: 20),
        ElevatedButton.icon(
          onPressed: _inCall ? _hangUp : null,
          icon: const Icon(Icons.call_end),
          label: const Text("Hang Up"),
          style: ElevatedButton.styleFrom(
            backgroundColor: Colors.red,
            padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 12),
          ),
        ),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.black,
      appBar: AppBar(
        title: const Text('Video Call'),
        backgroundColor: Colors.black87,
      ),
      body: Column(
        children: [
          Expanded(child: RTCVideoView(_localRenderer, mirror: true)),
          Expanded(child: RTCVideoView(_remoteRenderer)),
          const SizedBox(height: 16),
          _buildControlButtons(),
          const SizedBox(height: 24),
        ],
      ),
    );
  }
}
