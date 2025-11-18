import 'package:flutter/material.dart';
import 'package:flutter_webrtc/flutter_webrtc.dart';
import 'package:flutterapp01/main.dart';
import 'package:flutterapp01/GoogleSignIn.dart';
import 'package:google_sign_in/google_sign_in.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'userdata.dart';

Future<Map<String, dynamic>?> showCaregiverOnboarding(
    BuildContext context, String uid) async {
    final nameController = TextEditingController();
    final ageController = TextEditingController();
    final roleController = TextEditingController();

    String gender = "Female"; // default

    return await showDialog<Map<String, dynamic>>(
      context: context,
      barrierDismissible: false,
      builder: (context) {
        return StatefulBuilder(
          builder: (context, setState) {
            return AlertDialog(
              backgroundColor: const Color(0xFF162D41),
              shape: RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(16),
              ),
              title: const Text(
                "Tell us about you",
                style: TextStyle(color: Colors.white, fontWeight: FontWeight.bold),
              ),
              content: SingleChildScrollView(
                child: Column(
                  children: [
                    // NAME
                    TextField(
                      controller: nameController,
                      style: const TextStyle(color: Colors.white),
                      decoration: const InputDecoration(
                        labelText: "Name",
                        labelStyle: TextStyle(color: Colors.white70),
                        enabledBorder: UnderlineInputBorder(
                            borderSide: BorderSide(color: Colors.white54)),
                        focusedBorder: UnderlineInputBorder(
                            borderSide: BorderSide(color: Color(0xFFFDCD8B))),
                      ),
                    ),

                    // AGE
                    TextField(
                      controller: ageController,
                      style: const TextStyle(color: Colors.white),
                      keyboardType: TextInputType.number,
                      decoration: const InputDecoration(
                        labelText: "Age",
                        labelStyle: TextStyle(color: Colors.white70),
                        enabledBorder: UnderlineInputBorder(
                            borderSide: BorderSide(color: Colors.white54)),
                        focusedBorder: UnderlineInputBorder(
                            borderSide: BorderSide(color: Color(0xFFFDCD8B))),
                      ),
                    ),

                    const SizedBox(height: 20),

                    // GENDER DROPDOWN
                    DropdownButtonFormField<String>(
                      value: gender,
                      dropdownColor: const Color(0xFF162D41),
                      style: const TextStyle(color: Colors.white),
                      items: ["Female", "Male", "Other"]
                          .map((g) => DropdownMenuItem(
                                value: g,
                                child: Text(g,
                                    style: const TextStyle(color: Colors.white)),
                              ))
                          .toList(),
                      onChanged: (value) => setState(() => gender = value!),
                      decoration: const InputDecoration(
                        labelText: "Gender",
                        labelStyle: TextStyle(color: Colors.white70),
                        enabledBorder: UnderlineInputBorder(
                            borderSide: BorderSide(color: Colors.white54)),
                        focusedBorder: UnderlineInputBorder(
                            borderSide: BorderSide(color: Color(0xFFFDCD8B))),
                      ),
                    ),

                    // ROLE
                    TextField(
                      controller: roleController,
                      style: const TextStyle(color: Colors.white),
                      decoration: const InputDecoration(
                        labelText: "Role (e.g., Mother, Son…)",
                        labelStyle: TextStyle(color: Colors.white70),
                        enabledBorder: UnderlineInputBorder(
                            borderSide: BorderSide(color: Colors.white54)),
                        focusedBorder: UnderlineInputBorder(
                            borderSide: BorderSide(color: Color(0xFFFDCD8B))),
                      ),
                    ),
                  ],
                ),
              ),
              actions: [
                TextButton(
                  onPressed: () => Navigator.pop(context, null),
                  child:
                      const Text("Cancel", style: TextStyle(color: Colors.redAccent)),
                ),
                TextButton(
                  onPressed: () {
                    if (nameController.text.isEmpty ||
                        ageController.text.isEmpty ||
                        roleController.text.isEmpty) {
                      ScaffoldMessenger.of(context).showSnackBar(
                        const SnackBar(content: Text("Please fill all fields")),
                      );
                      return;
                    }

                    Navigator.pop(context, {
                      "name": nameController.text.trim(),
                      "age": ageController.text.trim(),
                      "gender": gender,
                      "role": roleController.text.trim(),
                      "uid": uid,
                    });
                  },
                  child: const Text("Confirm",
                      style: TextStyle(color: Color(0xFFFDCD8B))),
                ),
              ],
            );
          },
        );
      },
    );
  }


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
                onTap: () async {
                  try {
                    // 1️⃣ Sign in with Google
                    final UserCredential userCredential = await signInWithGoogle();  

                    if (userCredential.user == null) return; // safety check
                    final uid = userCredential.user!.uid;

                    // 2️⃣ Check if user already exists in Firestore
                    final userDoc = await FirebaseFirestore.instance
                        .collection('caregivers')
                        .doc(uid)
                        .get();

                    final bool isFirstTime = !userDoc.exists;

                    // 3️⃣ If FIRST TIME → show onboarding popup
                    if (isFirstTime) {
                      final caregiver = await showCaregiverOnboarding(context, uid);

                      // If user cancelled popup, don't continue
                      if (caregiver == null) return;

                      // 4️⃣ Save new caregiver into Firestore
                      await FirebaseFirestore.instance
                          .collection('caregivers')
                          .doc(uid)
                          .set(caregiver);
                    }

                    // 5️⃣ Continue to home page
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (_) => BuddyHomePage(uid: uid)),
                    );
                  } catch (e) {
                    // 6️⃣ Error handling
                    print("Google Sign-In Failed: $e");

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
