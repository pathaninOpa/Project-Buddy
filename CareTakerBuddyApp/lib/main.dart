import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutterapp01/SettingPage.dart';
import 'package:flutterapp01/userdata.dart';// Your CareGiver class
import 'package:intl/intl.dart';
import 'join_screen.dart';
import 'SplashPage.dart';
import 'package:firebase_core/firebase_core.dart';
import 'firebase_options.dart';
// improvement review: use คำนำหน้า like granny kat

void main() async { // <--- 1. Make main() asynchronous
  // 2. Ensure Flutter is ready to run bindings
  WidgetsFlutterBinding.ensureInitialized(); 

  // 3. Initialize Firebase before running the app
  await Firebase.initializeApp( 
    options: DefaultFirebaseOptions.currentPlatform,
  );

  // Keep your system UI settings
  SystemChrome.setSystemUIOverlayStyle(
    const SystemUiOverlayStyle(statusBarColor: Colors.transparent),
  );

  // 4. Run your root application widget
  runApp(const MaterialApp(
    debugShowCheckedModeBanner: false,
    // Use the widget name you prefer (e.g., BuddyApp or MyApp)
    home: BuddyApp(), 
  ));
}

/*
void main() {
  SystemChrome.setSystemUIOverlayStyle(
    const SystemUiOverlayStyle(statusBarColor: Colors.transparent),
  );
  runApp(const MaterialApp(
    debugShowCheckedModeBanner: false,
    home: BuddyApp(),
  ));
}
*/
class BuddyApp extends StatelessWidget {
  const BuddyApp({super.key});
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      home: SplashPage(),
    );
  }

  /*
  @override
  Widget build(BuildContext context) {
    final user = FirebaseAuth.instance.currentUser;
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      routes: {
        //'/': (context) => const SplashPage(),
        '/login': (context) => const LoginPage(),
        '/home': (context) => BuddyHomePage(
        uid: FirebaseAuth.instance.currentUser!.uid,
      ),
      },
    initialRoute: user == null ? '/login' : '/home',

    );
  }*/
}



class BuddyHomePage extends StatefulWidget {
  final String uid;

  const BuddyHomePage({super.key, required this.uid});

  @override
  State<BuddyHomePage> createState() => _BuddyHomePageState();
}

class _BuddyHomePageState extends State<BuddyHomePage> {
  String caregiverName = "";
  bool loading = true;

  @override
  void initState() {
    super.initState();
    loadCaregiver();
    _loadBuddiesFromFirestore();
  }

  Future<void> _loadBuddiesFromFirestore() async {
    try {
      final snapshot = await FirebaseFirestore.instance
          .collection('caregivers')
          .doc(widget.uid)            // <-- caregiver’s UID
          .collection('buddies')
          .get();

      setState(() {
        buddies = snapshot.docs
            .map((doc) => Buddy.fromMap(doc.data()))
            .toList();
        loadingBuddies = false;
      });
    } catch (e) {
      debugPrint('Error loading buddies: $e');
      setState(() {
        loadingBuddies = false;
      });
    }
  }

  Future<void> loadCaregiver() async {
    final doc = await FirebaseFirestore.instance
        .collection('caregivers')
        .doc(widget.uid)
        .get();

    if (doc.exists) {
      setState(() {
        caregiverName = doc['name'];
        loading = false;
      });
    }
  }

  


  List<Buddy> buddies = [];
  bool loadingBuddies = true; 

  /*
  void _addBuddy() {
    setState(() {
      buddies.add(Buddy(
        name: 'Buddy ${buddies.length + 1}',
        imagePath: 'assets/Buddy1.jpeg',
      ));
    });
  }*/

  void _addBuddy() async {
  final buddyData = await showAddBuddyPopup(context);

    if (buddyData == null) return; // user cancelled

    // Generate unique ID for this elder
    final buddyId = DateTime.now().millisecondsSinceEpoch.toString();

    // Save to Firestore
    await FirebaseFirestore.instance
        .collection('caregivers')
        .doc(widget.uid)
        .collection('buddies')
        .doc(buddyId)
        .set({
      "buddyID" : buddyId,
      "buddyName": 'Buddy ${buddies.length + 1}',
      "name": buddyData['name'],
      "age": buddyData['age'],
      "gender": buddyData['gender'],
      "role": buddyData['role'],
    });

    await showBuddyIdPopup(context, buddyId);

    setState(() {
      buddies.add(
        Buddy(
          buddyId: buddyId,
          buddyName: 'Buddy ${buddies.length + 1}',  // card title
          name: buddyData['name'],                   // elder name
          age: buddyData['age'],
          gender: buddyData['gender'],
          role: buddyData['role'],
        ),
      );
    });
  }

  Future<void> showBuddyIdPopup(BuildContext context, String buddyId) async {
    await showDialog(
      context: context,
      barrierDismissible: false,
      builder: (context) {
        return AlertDialog(
          backgroundColor: const Color(0xFF162D41),
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(16),
          ),
          title: const Text(
            "Buddy Created!",
            style: TextStyle(color: Colors.white, fontWeight: FontWeight.bold),
          ),
          content: Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              const Text(
                "Share this ID with your elder so they can connect to this Buddy:",
                style: TextStyle(color: Colors.white70),
                textAlign: TextAlign.center,
              ),
              const SizedBox(height: 12),
              Container(
                padding: const EdgeInsets.all(12),
                decoration: BoxDecoration(
                  color: Color(0xFF1F3A52),
                  borderRadius: BorderRadius.circular(8),
                ),
                child: SelectableText(
                  buddyId,
                  style: const TextStyle(
                      color: Color(0xFFFDCD8B), fontSize: 20, fontWeight: FontWeight.bold),
                  textAlign: TextAlign.center,
                ),
              ),
              const SizedBox(height: 10),
              TextButton.icon(
                onPressed: () {
                  Clipboard.setData(ClipboardData(text: buddyId));
                  ScaffoldMessenger.of(context).showSnackBar(
                    const SnackBar(content: Text("Copied to clipboard!")),
                  );
                },
                icon: const Icon(Icons.copy, color: Color(0xFFFDCD8B)),
                label: const Text(
                  "Copy Buddy ID",
                  style: TextStyle(color: Color(0xFFFDCD8B)),
                ),
              ),
            ],
          ),
          actions: [
            TextButton(
              onPressed: () => Navigator.pop(context),
              child: const Text(
                "Done",
                style: TextStyle(color: Color(0xFFFDCD8B)),
              ),
            ),
          ],
        );
      },
    );
  }


  Future<Map<String, dynamic>?> showAddBuddyPopup(BuildContext context) async {
  final nameController = TextEditingController();
  final ageController = TextEditingController();
  final roleController = TextEditingController();
  String gender = "Female";

  return await showDialog<Map<String, dynamic>>(
    context: context,
    barrierDismissible: false,
    builder: (context) {
      return StatefulBuilder(builder: (context, setState) {
        return AlertDialog(
          backgroundColor: const Color(0xFF162D41),
          shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16)),
          title: const Text(
            "Add Care Receiver",
            style: TextStyle(color: Colors.white, fontWeight: FontWeight.bold),
          ),
          content: SingleChildScrollView(
            child: Column(
              children: [
                TextField(
                  controller: nameController,
                  style: const TextStyle(color: Colors.white),
                  decoration: const InputDecoration(
                    labelText: "Name",
                    labelStyle: TextStyle(color: Colors.white70),
                    enabledBorder: UnderlineInputBorder(
                        borderSide: BorderSide(color: Colors.white54)),
                  ),
                ),
                TextField(
                  controller: ageController,
                  style: const TextStyle(color: Colors.white),
                  keyboardType: TextInputType.number,
                  decoration: const InputDecoration(
                    labelText: "Age",
                    labelStyle: TextStyle(color: Colors.white70),
                    enabledBorder: UnderlineInputBorder(
                        borderSide: BorderSide(color: Colors.white54)),
                  ),
                ),
                const SizedBox(height: 20),
                DropdownButtonFormField<String>(
                  initialValue: gender,
                  dropdownColor: const Color(0xFF162D41),
                  style: const TextStyle(color: Colors.white),
                  items: ["Female", "Male", "Other"]
                      .map((g) => DropdownMenuItem(
                            value: g,
                            child: Text(g, style: const TextStyle(color: Colors.white)),
                          ))
                      .toList(),
                  onChanged: (value) => setState(() => gender = value!),
                  decoration: const InputDecoration(
                    labelText: "Gender",
                    labelStyle: TextStyle(color: Colors.white70),
                  ),
                ),
                TextField(
                  controller: roleController,
                  style: const TextStyle(color: Colors.white),
                  decoration: const InputDecoration(
                    labelText: "Role (e.g., Grandma, Grandpa)",
                    labelStyle: TextStyle(color: Colors.white70),
                    enabledBorder: UnderlineInputBorder(
                        borderSide: BorderSide(color: Colors.white54)),
                  ),
                ),
              ],
            ),
          ),
          actions: [
            TextButton(
              onPressed: () => Navigator.pop(context),
              child: const Text("Cancel", style: TextStyle(color: Colors.redAccent)),
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
                  "name": nameController.text,
                  "age": ageController.text,
                  "gender": gender,
                  "role": roleController.text,
                });
              },
              child: const Text("Confirm", style: TextStyle(color: Color(0xFFFDCD8B))),
            ),
          ],
        );
      });
    },
  );
}


  
  @override
  Widget build(BuildContext context) {
  /*
    final caregiver = CareGiver(
      cgname: 'Jane Doe',
      cgage: '35',
      cggender: 'Female',
      cgrole: 'Mother',
    );
  */
  /*
    final carereceiver = CareReceiver(
      crname: 'Kat',
      crage: '90',
      crgender: 'Female',
      crrole: 'Grandmother',
      crbirthday: '1933-05-15',
    );
  */

    return Scaffold(
      resizeToAvoidBottomInset: true,
      backgroundColor: const Color.fromRGBO(22, 45, 65, 1),
     appBar: AppBar(
  backgroundColor: Colors.transparent,
  elevation: 0,
  automaticallyImplyLeading: false,

  // ⬇️ Move everything DOWN
  //toolbarHeight: 120,

  title: Padding(
    padding: const EdgeInsets.only(left: 23, top: 10), 
    // ↑ pushed down ~35px
    child: Row(
      crossAxisAlignment: CrossAxisAlignment.center,
      children: [

        // -------------------------
        //  WELCOME TEXT COLUMN
        // -------------------------
        Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text(
              'Welcome,',
              style: TextStyle(
                color: Color(0xFFFFAFA0), 
                fontSize: 20,
                fontWeight: FontWeight.w600,
              ),
            ),

            const SizedBox(height: 2),

            Text(
              caregiverName,
              style: const TextStyle(
                color: Colors.white,
                fontSize: 26,
                fontWeight: FontWeight.bold,
                height: 1.1,
              ),
            ),
          ],
        ),

        const Spacer(),

        // -------------------------
        //  SETTINGS ICON
        // -------------------------
        Padding(
          padding: const EdgeInsets.only(right: 12), // 👈 move left by 12 px
          child: IconButton(
            icon: const Icon(Icons.settings, color: Colors.white, size: 35),
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (_) => const SettingPage()),
              );
            },
          ),
        ),
      ],
    ),
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
                  children: [
                    Expanded(
                      child: ClipRRect(
                        borderRadius: BorderRadius.circular(16),
                        child: Image.asset(
                          buddy.imagePath,
                          fit: BoxFit.cover,
                        ),
                      ),
                    ),
                    const SizedBox(height: 8),
                    Text(
                      '${buddy.role} ${buddy.name}\'s Buddy',
                      maxLines: 2,
                      overflow: TextOverflow.ellipsis,
                      textAlign: TextAlign.center,
                      style: const TextStyle(
                        color: Colors.white,
                        fontSize: 14,
                        fontWeight: FontWeight.bold,
                      ),
                    )
                  ],
                ),

              );
            }),
            GestureDetector(
              onTap: _addBuddy,
              child: Column(
                mainAxisSize: MainAxisSize.min,
                children: [
                  //AspectRatio(
                    //aspectRatio: 1,
                     Container(
                      width: 135,
                      height: 135,
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
  //final CareReceiver carereceiver;

  const BuddyDetailPage({super.key, required this.buddy});

  @override
  State<BuddyDetailPage> createState() => _BuddyDetailPageState();
}

class _BuddyDetailPageState extends State<BuddyDetailPage> {
  String mood = 'happy'; // or 'neutral', 'sad'

  String getEmoji(String mood) {
    switch (mood) {
      case 'happy':
        return '😊';
      case 'neutral':
        return '🙂';
      case 'sad':
        return '😞';
      default:
        return '🙂';
    }
  }
  
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0xFF162D41),
      appBar: AppBar(
        leading: IconButton(
          icon: const Icon(Icons.navigate_before , color: Colors.white),
          onPressed: () => Navigator.pop(context),
        ),
        title: Text('${widget.buddy.role} ${widget.buddy.name}\'s Buddy',
                      style: TextStyle(color: Colors.white)),
        backgroundColor: const Color.fromARGB(0, 239, 235, 235),
        elevation: 0,
      ),
      body: Padding(
        padding: const EdgeInsets.all(24),
        child: Column(
          children: [
            //const SizedBox(height: 10),
            Text(
              getEmoji(mood),
              style: TextStyle(fontSize: 100),
            ),
            const SizedBox(height: 15),
            Text(
              '${widget.buddy.role} ${widget.buddy.name} is feeling $mood',
              style: const TextStyle(
                    color: Colors.white,
                    fontSize: 17,
                    fontWeight: FontWeight.bold,
                  ),
            ),
            const SizedBox(height: 15),
            Text(
              'last updated at ${DateFormat('hh:mm a').format(DateTime.now())} on ${DateFormat('dd/MM/yyyy').format(DateTime.now())}.',
              style: const TextStyle(
                    color: Color.fromARGB(255, 218, 218, 218),
                    fontSize: 10
                  ),
            ),
          
            
            const SizedBox(height: 35),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              
              children: [
                _actionButton(
                  icon: Icons.video_call,
                  label: 'Video Call',
                  color: const Color.fromARGB(255, 54, 137, 39),
                  onTap: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (_) => const JoinScreen()),
                    );
                  },
                ),
                _actionButton(
                  icon: Icons.notifications,
                  label: 'Reminder',
                  color: const Color.fromARGB(255, 255, 166, 34),
                  onTap: () {
                    final uid = FirebaseAuth.instance.currentUser!.uid; // <-- add this (import firebase_auth)

                    Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (_) => ReminderPage(
                          uid: uid,
                          buddy: widget.buddy, // pass the Buddy object from BuddyDetailPage
                        ),
                      ),
                    );
                  },
                ),
                _actionButton(
                  icon: Icons.health_and_safety,
                  label: 'Analysis',
                  color: const Color.fromARGB(255, 255, 157, 165),
                  onTap: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (_) => const AnalysisPage()),
                    );
                  },
                ),
              ],
            ),
            /*const SizedBox(height: 32),
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
            ),*/
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
class ReminderPage extends StatefulWidget {
  final String uid;     // caregiver uid
  final Buddy buddy;    // which buddy this reminder page belongs to

  const ReminderPage({
    super.key,
    required this.uid,
    required this.buddy,
  });

  @override
  State<ReminderPage> createState() => _ReminderPageState();
}

class _ReminderPageState extends State<ReminderPage> {
  final List<Event> _allEvents = []; // Stores ALL events across all dates.
  late DateTime _currentDate = DateTime.now();
  bool _loading = true;

  @override
  void initState() {
    super.initState();
    _currentDate = DateTime.now();
    _loadEventsFromFirestore();
  }

  // Load events from Firestore for this caregiver + buddy
  Future<void> _loadEventsFromFirestore() async {
    try {
      final snapshot = await FirebaseFirestore.instance
          .collection('caregivers')
          .doc(widget.uid)
          .collection('buddies')
          .doc(widget.buddy.buddyId)
          .collection('events')
          .orderBy('date')
          .orderBy('time')
          .get();

      final loadedEvents = snapshot.docs.map((doc) {
        final data = doc.data();
        return Event.fromMap(data);
      }).toList();

      setState(() {
        _allEvents
          ..clear()
          ..addAll(loadedEvents);
        _loading = false;
      });
    } catch (e) {
      debugPrint('Error loading events: $e');
      setState(() {
        _loading = false;
      });
    }
  }

  // Helper: compare only year, month, day
  bool _isSameDay(DateTime a, DateTime b) {
    return a.year == b.year && a.month == b.month && a.day == b.day;
  }

  // Add event to local list & keep sorted
  void _addEvent(Event newEvent) {
    setState(() {
      _allEvents.add(newEvent);
      _allEvents.sort((a, b) {
        final dateComparison = a.date.compareTo(b.date);
        if (dateComparison != 0) return dateComparison;
        return a.time.compareTo(b.time);
      });
    });
  }

  void _toggleAnnouncedStatus(Event event) {
    setState(() {
      event.isAnnounced = !event.isAnnounced;
    });
    // Optional: update in Firestore too
    // (you can add this later if you want)
  }

  void _onDaySelected(DateTime selectedDate) {
    setState(() {
      _currentDate = selectedDate;
    });
  }

  Widget _buildDayPickerRow() {
    return Row(
      mainAxisAlignment: MainAxisAlignment.spaceBetween,
      children: List.generate(7, (index) {
        final dateToDisplay = _currentDate.add(Duration(days: index - 3));

        final bool isSelected = _isSameDay(dateToDisplay, _currentDate);

        // This is just a highlight example; currently false most of the time
        final bool isSpecial = false;

        return _buildDayItem(
          DateFormat('EEE').format(dateToDisplay),
          dateToDisplay.day.toString(),
          isSelected,
          isSpecial,
          dateToDisplay,
        );
      }),
    );
  }

  Widget _buildDayItem(
      String day,
      String date,
      bool isSelected,
      bool isSpecial,
      DateTime fullDate,
      ) {
    return GestureDetector(
      onTap: () => _onDaySelected(fullDate),
      child: Column(
        children: [
          Text(
            day,
            style: TextStyle(color: Colors.grey[400], fontSize: 13),
          ),
          const SizedBox(height: 4),
          Container(
            padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 4),
            decoration: isSelected
                ? BoxDecoration(
                    color: Colors.blue.withOpacity(0.3),
                    borderRadius: BorderRadius.circular(8),
                  )
                : null,
            child: Text(
              date,
              style: TextStyle(
                fontWeight: FontWeight.bold,
                fontSize: 18,
                color: isSelected
                    ? Colors.blue
                    : (isSpecial ? Colors.orange : Colors.white),
              ),
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildEventCard(Event event, Function(Event) onToggleStatus) {
    return Card(
      margin: const EdgeInsets.symmetric(vertical: 8.0),
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(15)),
      color: Colors.white12,
      elevation: 3,
      child: Padding(
        padding: const EdgeInsets.all(15.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Row(
              children: [
                Expanded(
                  child: Text(
                    event.title,
                    style: const TextStyle(
                      fontWeight: FontWeight.bold,
                      fontSize: 18,
                      color: Colors.white,
                    ),
                  ),
                ),
                Text(
                  event.time,
                  style: const TextStyle(
                    fontSize: 16,
                    color: Colors.white70,
                  ),
                ),
              ],
            ),
            const SizedBox(height: 8),
            Text(
              event.description,
              style: const TextStyle(
                fontSize: 14,
                color: Colors.white70,
              ),
            ),
            const SizedBox(height: 10),
            Align(
              alignment: Alignment.bottomRight,
              child: ElevatedButton(
                onPressed: () => onToggleStatus(event),
                style: ElevatedButton.styleFrom(
                  backgroundColor:
                      event.isAnnounced ? Colors.orange : Colors.blue,
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(8),
                  ),
                ),
                child: Text(
                  event.isAnnounced ? 'Move to Wait' : 'Announce',
                  style: const TextStyle(color: Colors.white),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    if (_loading) {
      return const Scaffold(
        backgroundColor: Color(0xFF162D41),
        body: Center(
          child: CircularProgressIndicator(color: Colors.white),
        ),
      );
    }

    final List<Event> dailyEvents = _allEvents.where((event) {
      return _isSameDay(event.date, _currentDate);
    }).toList();

    final List<Event> announcedEvents =
        dailyEvents.where((event) => event.isAnnounced).toList();
    final List<Event> waitingEvents =
        dailyEvents.where((event) => !event.isAnnounced).toList();

    return Scaffold(
      backgroundColor: const Color(0xFF162D41),
      appBar: AppBar(
        leading: IconButton(
          icon: const Icon(Icons.navigate_before, color: Colors.white),
          onPressed: () => Navigator.pop(context),
        ),
        title: const Text(
          'Reminders',
          style: TextStyle(color: Colors.white),
        ),
        backgroundColor: Colors.transparent,
        elevation: 0,
      ),
      body: Padding(
        padding: const EdgeInsets.all(20.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              DateFormat('MMMM d, yyyy').format(_currentDate),
              style: const TextStyle(color: Colors.white70, fontSize: 16),
            ),
            const SizedBox(height: 8),
            Text(
              DateFormat('EEEE').format(_currentDate),
              style: const TextStyle(
                fontWeight: FontWeight.bold,
                fontSize: 32,
                color: Colors.white,
              ),
            ),
            const SizedBox(height: 20),
            _buildDayPickerRow(),
            const SizedBox(height: 30),
            const Text(
              'Announced',
              style: TextStyle(
                fontWeight: FontWeight.bold,
                fontSize: 22,
                color: Colors.white,
              ),
            ),
            const SizedBox(height: 10),
            Expanded(
              child: announcedEvents.isEmpty
                  ? const Center(
                      child: Text(
                        'No announced events for this day.',
                        style: TextStyle(fontSize: 16, color: Colors.white54),
                      ),
                    )
                  : ListView.builder(
                      itemCount: announcedEvents.length,
                      itemBuilder: (context, index) {
                        return _buildEventCard(
                          announcedEvents[index],
                          _toggleAnnouncedStatus,
                        );
                      },
                    ),
            ),
            const SizedBox(height: 20),
            const Text(
              'Wait to Announce',
              style: TextStyle(
                fontWeight: FontWeight.bold,
                fontSize: 22,
                color: Colors.white,
              ),
            ),
            const SizedBox(height: 10),
            Expanded(
              child: waitingEvents.isEmpty
                  ? const Center(
                      child: Text(
                        'No events waiting for this day.',
                        style: TextStyle(fontSize: 16, color: Colors.white54),
                      ),
                    )
                  : ListView.builder(
                      itemCount: waitingEvents.length,
                      itemBuilder: (context, index) {
                        return _buildEventCard(
                          waitingEvents[index],
                          _toggleAnnouncedStatus,
                        );
                      },
                    ),
            ),
          ],
        ),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: () async {
          final newEvent = await Navigator.of(context).push<Event>(
            MaterialPageRoute(
              builder: (context) => AddEventScreen(
                uid: widget.uid,
                buddy: widget.buddy,
              ),
            ),
          );

          if (newEvent != null) {
            _addEvent(newEvent);
          }
        },
        backgroundColor: const Color(0xFFFDCD8B),
        child: const Icon(Icons.add),
      ),
    );
  }
}
class AddEventScreen extends StatefulWidget {
  final String uid;      // caregiver UID
  final Buddy buddy;     // full buddy object (contains buddyId)

  const AddEventScreen({
    super.key,
    required this.uid,
    required this.buddy,
  });

  @override
  State<AddEventScreen> createState() => _AddEventScreenState();
}

class _AddEventScreenState extends State<AddEventScreen> {
  final _formKey = GlobalKey<FormState>();
  String _title = '';
  String _description = '';
  DateTime _selectedDate = DateTime.now();
  TimeOfDay _selectedTime = TimeOfDay.now();

  Future<void> _pickDate() async {
    final DateTime? picked = await showDatePicker(
      context: context,
      initialDate: _selectedDate,
      firstDate: DateTime(2000),
      lastDate: DateTime(2100),
      builder: (context, child) {
        return Theme(
          data: ThemeData.dark().copyWith(
            colorScheme: const ColorScheme.dark(
              primary: Color(0xFFFDCD8B),
              onPrimary: Colors.black,
              surface: Color(0xFF162D41),
              onSurface: Colors.white,
            ), dialogTheme: DialogThemeData(backgroundColor: const Color(0xFF162D41)),
          ),
          child: child!,
        );
      },
    );
    if (picked != null && picked != _selectedDate) {
      setState(() {
        _selectedDate = picked;
      });
    }
  }

  Future<void> _pickTime() async {
    final TimeOfDay? picked = await showTimePicker(
      context: context,
      initialTime: _selectedTime,
      initialEntryMode: TimePickerEntryMode.input,
      builder: (context, child) {
        return Theme(
          data: ThemeData.dark().copyWith(
            colorScheme: const ColorScheme.dark(
              primary: Color(0xFFFDCD8B),
              onPrimary: Colors.black,
              surface: Color(0xFF162D41),
              onSurface: Colors.white,
            ), dialogTheme: DialogThemeData(backgroundColor: const Color(0xFF162D41)),
          ),
          child: child!,
        );
      },
    );
    if (picked != null && picked != _selectedTime) {
      setState(() {
        _selectedTime = picked;
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0xFF162D41),
      appBar: AppBar(
        title: const Text(
          "Add New Event",
          style: TextStyle(color: Colors.white),
        ),
        backgroundColor: Colors.transparent,
        elevation: 0,
        iconTheme: const IconThemeData(color: Colors.white),
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Form(
          key: _formKey,
          child: ListView(
            children: <Widget>[
              TextFormField(
                decoration: InputDecoration(
                  labelText: 'Event Title',
                  labelStyle: const TextStyle(color: Colors.white70),
                  enabledBorder: OutlineInputBorder(
                    borderSide: const BorderSide(color: Colors.white54),
                    borderRadius: BorderRadius.circular(8.0),
                  ),
                  focusedBorder: OutlineInputBorder(
                    borderSide: const BorderSide(color: Color(0xFFFDCD8B)),
                    borderRadius: BorderRadius.circular(8.0),
                  ),
                  errorBorder: OutlineInputBorder(
                    borderSide: const BorderSide(color: Color(0xFFFF5252)),
                    borderRadius: BorderRadius.circular(8.0),
                  ),
                  focusedErrorBorder: OutlineInputBorder(
                    borderSide: const BorderSide(color: Color(0xFFFF5252)),
                    borderRadius: BorderRadius.circular(8.0),
                  ),
                ),
                style: const TextStyle(color: Colors.white),
                validator: (value) {
                  if (value == null || value.isEmpty) {
                    return 'Please enter a title';
                  }
                  return null;
                },
                onSaved: (value) {
                  _title = value!;
                },
              ),
              const SizedBox(height: 16.0),
              TextFormField(
                decoration: InputDecoration(
                  labelText: 'Description',
                  labelStyle: const TextStyle(color: Colors.white70),
                  enabledBorder: OutlineInputBorder(
                    borderSide: const BorderSide(color: Colors.white54),
                    borderRadius: BorderRadius.circular(8.0),
                  ),
                  focusedBorder: OutlineInputBorder(
                    borderSide: const BorderSide(color: Color(0xFFFDCD8B)),
                    borderRadius: BorderRadius.circular(8.0),
                  ),
                  errorBorder: OutlineInputBorder(
                    borderSide: const BorderSide(color: Color(0xFFFF5252)),
                    borderRadius: BorderRadius.circular(8.0),
                  ),
                  focusedErrorBorder: OutlineInputBorder(
                    borderSide: const BorderSide(color: Color(0xFFFF5252)),
                    borderRadius: BorderRadius.circular(8.0),
                  ),
                ),
                style: const TextStyle(color: Colors.white),
                maxLines: 3,
                onSaved: (value) {
                  _description = value ?? '';
                },
              ),
              const SizedBox(height: 16.0),
              ListTile(
                title: Text(
                  'Date: ${DateFormat('yyyy-MM-dd').format(_selectedDate)}',
                  style: const TextStyle(color: Colors.white),
                ),
                trailing: const Icon(Icons.calendar_today, color: Color(0xFFFDCD8B)),
                onTap: _pickDate,
              ),
              const SizedBox(height: 8.0),
              ListTile(
                title: Text(
                  'Time: ${_selectedTime.format(context)}',
                  style: const TextStyle(color: Colors.white),
                ),
                trailing: const Icon(Icons.access_time, color: Color(0xFFFDCD8B)),
                onTap: _pickTime,
              ),
              const SizedBox(height: 32.0),
              Center(
                child: ElevatedButton(
                  style: ElevatedButton.styleFrom(
                    backgroundColor: const Color(0xFFFDCD8B),
                    foregroundColor: Colors.black,
                    padding: const EdgeInsets.symmetric(
                        horizontal: 40, vertical: 15),
                    shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(30.0),
                    ),
                    elevation: 5,
                  ),
                  onPressed: () async {
                    if (_formKey.currentState!.validate()) {
                      _formKey.currentState!.save();

                      try {
                        final eventData = {
                          'title': _title,
                          'description': _description,
                          'date': Timestamp.fromDate(_selectedDate),
                          'time': _selectedTime.format(context),
                          'isAnnounced': false,
                          'createdAt': Timestamp.now(),
                        };

                        await FirebaseFirestore.instance
                            .collection('caregivers')
                            .doc(widget.uid)
                            .collection('buddies')
                            .doc(widget.buddy.buddyId)
                            .collection('events')
                            .add(eventData);

                        final newEvent = Event(
                          title: _title,
                          description: _description,
                          date: _selectedDate,
                          time: _selectedTime.format(context),
                          isAnnounced: false,
                        );

                        Navigator.of(context).pop(newEvent);
                      } catch (e) {
                        ScaffoldMessenger.of(context).showSnackBar(
                          SnackBar(content: Text("Error saving event: $e")),
                        );
                      }
                    }
                  },
                  child: const Text(
                    "Save Event",
                    style: TextStyle(
                      fontSize: 18,
                      fontWeight: FontWeight.bold,
                    ),
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

class AnalysisPage extends StatelessWidget {   
  const AnalysisPage({super.key});
  
  String getEmoji(String mood) {
    switch (mood) {
      case 'happy':
        return '😊';
      case 'neutral':
        return '🙂';
      case 'sad':
        return '😞';
      default:
        return '🙂';
    }
  }
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0xFF162D41), // Dark background
      appBar: AppBar(
        title: const Text(
          "Emotion Analysis",
          style: TextStyle(color: Colors.white), // White title
        ),
        backgroundColor: Colors.transparent, // Transparent app bar
        elevation: 0, // No shadow
        iconTheme: const IconThemeData(color: Colors.white), // White back arrow
      ),
      body: ListView(
        padding: const EdgeInsets.all(16.0),
        children: <Widget>[
          // Section 1: Overall Emotional Snapshot
          _buildSectionTitle(context, "Overall Emotional Snapshot"),
          _buildCard(
            context,
            Column(
              children: [
                Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: const [
                    Icon(Icons.sentiment_satisfied_alt,
                        color: Color(0xFFFDCD8B), size: 60), // Gold accent
                    SizedBox(width: 16),
                    Text(
                      "Generally Positive",
                      style: TextStyle(
                          fontSize: 24,
                          fontWeight: FontWeight.bold,
                          color: Colors.white),
                    ),
                  ],
                ),
                const SizedBox(height: 16),
                _buildInfoRow(
                    "Last Interaction:", "Yesterday, 3:45 PM (Positive)"),
                _buildInfoRow("Quick Alerts:", "No immediate concerns."),
              ],
            ),
          ),
          const SizedBox(height: 24),

          // Section 2: Emotional Trends Over Time
          _buildSectionTitle(context, "Emotional Trends Over Time"),
          _buildCard(
            context,
                _buildChartPlaceholder(
                    "Sentiment Trend (Last 30 Days)", Icons.show_chart),
            ),
          const SizedBox(height: 24),
          // Section 3: Key Themes & Topics
          _buildSectionTitle(context, "Key Themes & Topics"),
          _buildCard(
            context,
            Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                _buildInfoRow("Most Frequent Words:",
                    "Family, garden, doctor, sleep, memories"),
                const SizedBox(height: 8),
                _buildInfoRow("Emotionally Charged Topics:",
                    "Health issues (Anxiety), Grandchildren (Joy), Past events (Nostalgia)"),
                const SizedBox(height: 8),
                _buildInfoRow(
                    "Somatic Complaints (Last Week):", "Pain (3), Tired (5)"),
              ],
            ),
          ),
          const SizedBox(height: 24),

          // Section 4: Detailed Event Log / Alert History
          _buildSectionTitle(context, "Detailed Event Log"),
          _buildCard(
            context,
            Column(
              children: [
                _buildEventLogEntry(
                    "Jul 5, 2025 - 10:00 AM", "Increased sadness detected.",
                    severity: "Medium"),
                _buildEventLogEntry(
                    "Jul 4, 2025 - 02:15 PM", "Repeated mention of 'being alone'.",
                    severity: "High"),
                _buildEventLogEntry(
                    "Jul 3, 2025 - 09:30 AM", "Expressed joy about garden.",
                    severity: "Low"),
              ],
            ),
          ),
          const SizedBox(height: 24),

          // Section 5: Recommendations & Actionable Insights
      
          const SizedBox(height: 24),
        ],
      ),
    );
  }

  // Helper function to build section titles
  Widget _buildSectionTitle(BuildContext context, String title) {
    return Padding(
      padding: const EdgeInsets.only(bottom: 12.0),
      child: Text(
        title,
        style: const TextStyle(
          color: Color.fromARGB(255, 255, 255, 255), // Gold accent for titles
          fontSize: 20,
          fontWeight: FontWeight.bold,
        ),
      ),
    );
  }

  // Helper function to build a consistent card style
  Widget _buildCard(BuildContext context, Widget child) {
    return Card(
      color: const Color(0xFF1F3F5A), // Slightly lighter dark blue for cards
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(12.0),
      ),
      elevation: 4,
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: child,
      ),
    );
  }

  // Helper function for info rows within cards
  Widget _buildInfoRow(String label, String value) {
    return Padding(
      padding: const EdgeInsets.symmetric(vertical: 4.0),
      child: Row(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Text(
            label,
            style: const TextStyle(
                color: Colors.white70, fontWeight: FontWeight.bold),
          ),
          const SizedBox(width: 8),
          Expanded(
            child: Text(
              value,
              style: const TextStyle(color: Colors.white),
            ),
          ),
        ],
      ),
    );
  }

  // Helper function for chart placeholders
  Widget _buildChartPlaceholder(String title, IconData icon) {
    return Container(
      height: 150,
      decoration: BoxDecoration(
        color: const Color(0xFF2C5070), // Even lighter blue for chart areas
        borderRadius: BorderRadius.circular(8.0),
        border: Border.all(color: const Color(0xFFFDCD8B), width: 1), // Gold border
      ),
      alignment: Alignment.center,
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          Icon(icon, color: Colors.white54, size: 40),
          const SizedBox(height: 8),
          Text(
            "[$title Placeholder]",
            style: const TextStyle(color: Colors.white54, fontSize: 16),
            textAlign: TextAlign.center,
          ),
          const Text(
            "Real charts would go here.",
            style: TextStyle(color: Colors.white38, fontSize: 12),
            textAlign: TextAlign.center,
          ),
        ],
      ),
    );
  }

  // Helper function for event log entries
  Widget _buildEventLogEntry(String timestamp, String event,
      {String severity = "Low"}) {
    Color severityColor;
    switch (severity) {
      case "High":
        severityColor = const Color(0xFFFF5252); // Red
        break;
      case "Medium":
        severityColor = Colors.orange;
        break;
      default:
        severityColor = Colors.green;
    }

    return Padding(
      padding: const EdgeInsets.symmetric(vertical: 6.0),
      child: Row(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Text(
            timestamp,
            style: const TextStyle(color: Colors.white70, fontSize: 12),
          ),
          const SizedBox(width: 12),
          Expanded(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(
                  event,
                  style: const TextStyle(color: Colors.white, fontSize: 14),
                ),
                Text(
                  "Severity: $severity",
                  style: TextStyle(color: severityColor, fontSize: 12),
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}
