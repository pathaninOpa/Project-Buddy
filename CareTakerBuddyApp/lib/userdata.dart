// =============================
//  FIRESTORE MODELS
// =============================
import 'package:cloud_firestore/cloud_firestore.dart';


// =============================
//  CARE GIVER MODEL
// =============================
class CareGiver {
  CareGiver({
    required this.name,
    required this.age,
    required this.gender,
    required this.role,
  });

  String name;
  String age;
  String gender;
  String role;

  // Original method (kept)
  void caregiverInfo() {
    name = name;
    age = age;
    gender = gender;
    role = role;
  }

  // Firestore → Model
  factory CareGiver.fromMap(Map<String, dynamic> map) {
    return CareGiver(
      name: map['name'] ?? '',
      age: map['age'] ?? '',
      gender: map['gender'] ?? '',
      role: map['role'] ?? '',
    );
  }

  // Model → Firestore
  Map<String, dynamic> toMap() {
    return {
      'name': name,
      'age': age,
      'gender': gender,
      'role': role,
    };
  }
}



// =============================
//  BUDDY MODEL (Elder info stored in Buddy)
// =============================
class Buddy {
  String buddyId;    
  String buddyName;  
  String name;       // elder name
  String age;
  String gender;
  String role;
  String imagePath;

  Buddy({
    required this.buddyId,
    required this.buddyName,
    required this.name,
    required this.age,
    required this.gender,
    required this.role,
    this.imagePath = 'assets/Buddy1.jpeg',
  });

  // Firestore → Model
  factory Buddy.fromMap(Map<String, dynamic> map) {
  return Buddy(
    buddyId: map['buddyId'] ?? map['buddyID'] ?? '',
    buddyName: map['buddyName'] ?? '',
    name: map['name'] ?? '',
    age: map['age'] ?? '',
    gender: map['gender'] ?? '',
    role: map['role'] ?? '',
    imagePath: map['imagePath'] ?? 'assets/Buddy1.jpeg',
  );
}

  // Model → Firestore
  Map<String, dynamic> toMap() {
    return {
      'buddyId': buddyId,
      'buddyName': buddyName,
      'name': name,
      'age': age,
      'gender': gender,
      'role': role,
      'imagePath': imagePath,
    };
  }
}

// =============================
//  EVENT MODEL
// =============================
class Event {
  String title;
  String description;
  String time;
  DateTime date;
  bool isAnnounced;

  Event({
    required this.title,
    required this.description,
    required this.time,
    required this.date,
    required this.isAnnounced,
  });

  // Firestore → Model
  factory Event.fromMap(Map<String, dynamic> map) {
    return Event(
      title: map['title'] ?? '',
      description: map['description'] ?? '',
      time: map['time'] ?? '',
      date: (map['date'] as Timestamp).toDate(),
      isAnnounced: map['isAnnounced'] ?? false,
    );
  }

  // Model → Firestore
  Map<String, dynamic> toMap() {
    return {
      'title': title,
      'description': description,
      'time': time,
      'date': date,
      'isAnnounced': isAnnounced,
    };
  }
}





/*class CareReceiver {
  CareReceiver({
    required this.crname,
    required this.crage,
    required this.crbirthday,
    required this.crgender,
    required this.crrole
  });

  final String crname; 
  final String crage; 
  final String crbirthday;
  final String crgender; 
  final String crrole;

  void carereceiverInfo(){
      final crname = this.crname; 
      final crage = this.crage; 
      final crbirthday = this.crbirthday; 
      final crgender = this.crgender;
      final crrole = this.crrole;
  }
}

class CareGiver{
  CareGiver({
    required this.cgname,
    required this.cgage,
    required this.cggender,
    required this.cgrole
  });

  String cgname;
  String cgage;
  String cggender;
  String cgrole;

  void caregiverInfo(){
      cgname = cgname; 
      cgage = cgage; 
      cggender = cggender;
      cgrole = cgrole;
  }
}

class Buddy {
  String name;
  String imagePath;

  Buddy({required this.name, required this.imagePath});
}


class Event {
  String title;
  String description;
  String time; 
  DateTime date;
  bool isAnnounced;
 
  Event({
    required this.title,
    required this.description,
    required this.time,
    required this.date,
    required this.isAnnounced});
}

*/