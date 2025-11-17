import 'package:flutter/material.dart';
import 'package:flutter_webrtc/flutter_webrtc.dart';
import 'package:flutterapp01/main.dart';
import 'package:flutterapp01/GoogleSignIn.dart';
import 'package:google_sign_in/google_sign_in.dart';
import 'package:firebase_auth/firebase_auth.dart';

class LoginPage extends StatelessWidget {
  const LoginPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0xFF131C3A),

      body: Center(
        child: Container(
          padding: const EdgeInsets.all(25),
          margin: const EdgeInsets.symmetric(horizontal: 20),
          decoration: BoxDecoration(
            color: Colors.white,
            borderRadius: BorderRadius.circular(35),
          ),

          child: Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              // Title
              Text(
                "Welcome to Buddy",
                style: TextStyle(
                  fontSize: 26,
                  fontWeight: FontWeight.bold,
                  color: const Color(0xFF131C3A),
                ),
              ),

              const SizedBox(height: 30),

              // Google Sign In Button (Material style)
              GestureDetector(
                onTap: () async { // 1. Make the callback async
                  try {
                    // 2. Call your asynchronous sign-in function and await its result
                    final UserCredential userCredential = await signInWithGoogle(); 

                    // 3. Check if the sign-in was successful (e.g., if a user is returned)
                    if (userCredential.user != null) {
                      
                      // 4. Navigate to the BuddyHomePage on success
                      Navigator.push(
                        context,
                        MaterialPageRoute(builder: (_) => const BuddyHomePage()),
                      );
                      
                    }
                  } catch (e) {
                    // 5. Handle errors (e.g., user cancels sign-in, network error, etc.)
                    print("Google Sign-In Failed: $e");
                    
                    // Optionally show a SnackBar or AlertDialog to the user
                    ScaffoldMessenger.of(context).showSnackBar(
                      const SnackBar(content: Text("Sign-in failed. Please try again.")),
                    );
                  }
                },
                child: Container(
                  height: 50,
                  decoration: BoxDecoration(
                    color: Colors.white,
                    borderRadius: BorderRadius.circular(4),
                    border: Border.all(color: Colors.grey.shade300),
                    boxShadow: [
                      BoxShadow(
                        color: Colors.black.withOpacity(0.05),
                        blurRadius: 4,
                        offset: const Offset(0, 2),
                      )
                    ],
                  ),
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [

                      const SizedBox(width: 16),

                      // Button text
                      const Text(
                        "Sign in with Google",
                        style: TextStyle(
                          color: Colors.black54,
                          fontSize: 16,
                          fontWeight: FontWeight.w500,
                        ),
                      ),
                    ],
                  ),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
