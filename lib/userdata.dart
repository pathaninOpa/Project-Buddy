class CareReceiver {
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
      cgname = this.cgname; 
      cgage = this.cgage; 
      cggender = this.cggender;
      cgrole = this.cgrole;
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