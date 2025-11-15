import 'package:flutter/material.dart';
import 'dart:async';

class SplashPage extends StatefulWidget {
  const SplashPage({super.key});

  @override
  State<SplashPage> createState() => _SplashPageState();
}

class _SplashPageState extends State<SplashPage> {
  @override
  void initState() {
    super.initState();
    Timer(const Duration(seconds: 5), () {
      Navigator.pushReplacementNamed(context, '/login');
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0xFF131C3A),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Text(
              "Buddy",
              style: TextStyle(
                fontSize: 60,
                fontWeight: FontWeight.bold,
                color: const Color(0xFFE7C590),
              ),
            ),
            const SizedBox(height: 5),
            Container(
              width: 200,
              height: 3,
              color: Colors.lightBlueAccent,
            ),
            const SizedBox(height: 5),
            Text(
              "your robot companion",
              style: TextStyle(
                fontSize: 18,
                color: const Color(0xFFE7C590),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
