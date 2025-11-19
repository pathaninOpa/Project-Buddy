import 'package:flutter/material.dart';
import 'dart:async';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:flutterapp01/main.dart';
import 'LoginPage.dart';

class SplashPage extends StatefulWidget {
  const SplashPage({super.key});

  @override
  State<SplashPage> createState() => _SplashPageState();
}

class _SplashPageState extends State<SplashPage> {
  @override
  void initState() {
    super.initState();
    _goNext();
  }

  void _goNext() {
    Timer(const Duration(seconds: 5), () {
      final user = FirebaseAuth.instance.currentUser;

      if (user == null) {
        // Not logged in → go to login
        Navigator.pushReplacement(
          context,
          MaterialPageRoute(builder: (_) => const LoginPage()),
        );
      } else {
        // Already logged in → go to home
        Navigator.pushReplacement(
          context,
          MaterialPageRoute(builder: (_) => BuddyHomePage(uid: user.uid)),
        );
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color.fromRGBO(22, 45, 65, 1),
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
