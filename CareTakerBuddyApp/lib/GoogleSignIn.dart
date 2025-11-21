import 'package:google_sign_in/google_sign_in.dart';
import 'package:firebase_auth/firebase_auth.dart';

Future<UserCredential> signInWithGoogle() async {
  // Trigger the authentication flow
  final GoogleSignInAccount googleUser = await GoogleSignIn.instance.authenticate();

  // 1. ADD NULL CHECK HERE
  if (googleUser == null) {
    // If the user cancels the sign-in prompt, we exit the function.
    // You can choose to throw an exception, return null, or return a UserCredential
    // indicating failure. Throwing is often best for handling in the caller function (onTap).
    throw Exception("Google Sign-In was cancelled or failed.");
  }

  // Obtain the auth details from the request (Now googleUser is guaranteed NOT null)
  final GoogleSignInAuthentication googleAuth = googleUser.authentication; 

  // Create a new credential
  final credential = GoogleAuthProvider.credential(idToken: googleAuth.idToken);

  // Once signed in, return the UserCredential
  return await FirebaseAuth.instance.signInWithCredential(credential);
}