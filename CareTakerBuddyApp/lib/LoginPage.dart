import 'package:flutter/material.dart';
import 'package:flutterapp01/main.dart';
import 'package:flutterapp01/GoogleSignIn.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:cloud_firestore/cloud_firestore.dart';

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
              backgroundColor: const Color.fromRGBO(22, 45, 65, 1),
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
                      initialValue: gender,
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
      backgroundColor: const Color(0xFF162D41), // DARK BACKGROUND

      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [

            // =============================
            //  BUDDY LOGO BOX (Like your mock)
            // =============================
            Container(
              padding: const EdgeInsets.all(40),
              decoration: BoxDecoration(
                color: const Color(0xFF162D41),   // Inner dark box
                borderRadius: BorderRadius.circular(20),
              ),
              child: Column(
                children: const [
                  Text(
                    "Buddy",
                    style: TextStyle(
                      color: Color(0xFFE7C590),
                      fontSize: 70,
                      fontWeight: FontWeight.bold,
                    ),
                  ),
                  SizedBox(height: 10),
                  Text(
                    "your robot companion",
                    style: TextStyle(
                      color: Color(0xFFFFAFA0),
                      fontSize: 20,
                    ),
                  ),
                ],
              ),
            ),

            const SizedBox(height: 120),

            // =============================
            //  LOGIN BUTTON (WHITE, ROUNDED)
            // =============================
            GestureDetector(
              onTap: () async {
                try {
                  final UserCredential userCredential =
                      await signInWithGoogle();

                  if (userCredential.user == null) return;
                  final uid = userCredential.user!.uid;

                  // Check Firestore
                  final userDoc = await FirebaseFirestore.instance
                      .collection('caregivers')
                      .doc(uid)
                      .get();

                  final bool isFirstTime = !userDoc.exists;

                  // First-time popup
                  if (isFirstTime) {
                    final caregiver =
                        await showCaregiverOnboarding(context, uid);
                    if (caregiver == null) return;

                    await FirebaseFirestore.instance
                        .collection('caregivers')
                        .doc(uid)
                        .set(caregiver);
                  }

                  Navigator.push(
                    context,
                    MaterialPageRoute(
                      builder: (_) => BuddyHomePage(uid: uid),
                    ),
                  );
                } catch (e) {
                  print("Google Sign-In Failed: $e");

                  ScaffoldMessenger.of(context).showSnackBar(
                    const SnackBar(
                      content: Text("Sign-in failed. Please try again."),
                    ),
                  );
                }
              },

              child: Container(
                padding:
                    const EdgeInsets.symmetric(horizontal: 40, vertical: 14),
                decoration: BoxDecoration(
                  color: Colors.white,
                  borderRadius: BorderRadius.circular(25),
                  boxShadow: [
                    BoxShadow(
                      color: Colors.black.withOpacity(0.15),
                      blurRadius: 8,
                      offset: const Offset(0, 3),
                    )
                  ],
                ),
                child: const Text(
                  "Continue with google",
                  style: TextStyle(
                    fontSize: 20,
                    color: Color(0xFF162D41),
                    fontWeight: FontWeight.w600,
                  ),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
