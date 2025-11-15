import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutterapp01/userdata.dart';// Your CareGiver class
import 'dart:convert';
import 'package:web_socket_channel/io.dart';
import 'package:intl/intl.dart';
import 'package:permission_handler/permission_handler.dart';
import 'call_screen.dart';
import 'join_screen.dart';
import 'SplashPage.dart';
import 'LoginPage.dart';
// improvement review: use คำนำหน้า like granny kat

void main() {
  SystemChrome.setSystemUIOverlayStyle(
    const SystemUiOverlayStyle(statusBarColor: Colors.transparent),
  );
  runApp(const MaterialApp(
    debugShowCheckedModeBanner: false,
    home: BuddyApp(),
  ));
}

class BuddyApp extends StatelessWidget {
  const BuddyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      routes: {
        '/': (context) => const SplashPage(),
        '/login': (context) => const LoginPage(),
      },
      initialRoute: '/',
    );
  }
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
  
    final carereceiver = CareReceiver(
      crname: 'Kat',
      crage: '90',
      crgender: 'Female',
      crrole: 'Grandmother',
      crbirthday: '1933-05-15',
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
                      builder: (context) => BuddyDetailPage(buddy: buddy, carereceiver: carereceiver),
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
                    SizedBox(
                      width: 135,
                      height: 135,
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
  final CareReceiver carereceiver;

  const BuddyDetailPage({super.key, required this.buddy, required this.carereceiver});

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
        title: Text('${widget.carereceiver.crname}\'s Buddy',
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
              '${widget.carereceiver.crname} is feeling $mood',
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
                  icon: Icons.notifications ,
                  label: 'Reminder',
                  color: const Color.fromARGB(255, 255, 166, 34),
                  onTap: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (_) => const ReminderPage()),
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
  const ReminderPage({super.key});

  @override
  State<ReminderPage> createState() => _ReminderPageState();
}

class _ReminderPageState extends State<ReminderPage> {


  final List<Event> _allEvents = []; // Stores ALL events across all dates.
  
  // The 'late' variable that caused the error.
  late DateTime _currentDate = DateTime.now(); // Example

  @override
  void initState() {
    super.initState();
    // **THE FIX**: Initialize _currentDate here before it's ever used.
    _currentDate = DateTime.now();
  }

  // Function to add a new event to the list and sort it
  void _addEvent(Event newEvent) {
    setState(() {
      _allEvents.add(newEvent);
      _allEvents.sort((a, b) {
        // First sort by date, then by time
        int dateComparison = a.date.compareTo(b.date);
        if (dateComparison != 0) return dateComparison;
        return a.time.compareTo(b.time); // Simple string comparison for "HH:mm"
      });
    });
  }

  // Function to toggle the announced status of an event
  void _toggleAnnouncedStatus(Event event) {
    setState(() {
      event.isAnnounced = !event.isAnnounced;
      // Re-sorting is not strictly necessary here but maintains order if needed elsewhere
    });
  }

  // Function to handle when a user taps on a day in the picker
  void _onDaySelected(DateTime selectedDate) {
    setState(() {
      _currentDate = selectedDate;
    });
  }

  // Function to build the dynamic row of selectable days
  Widget _buildDayPickerRow() {
    return Row(
      mainAxisAlignment: MainAxisAlignment.spaceBetween,
      children: List.generate(7, (index) {
        // Generate 7 days: 3 before current, current day, 3 after current
        final dateToDisplay = _currentDate.add(Duration(days: index - 3));

        // Check if the generated day is the same as the selected day
        final bool isSelected = dateToDisplay.day == _currentDate.day &&
            dateToDisplay.month == _currentDate.month &&
            dateToDisplay.year == _currentDate.year;
        
        // Example of a "special" day, e.g., Saturday
        final bool isSpecial = dateToDisplay.weekday == DateTime.now();

        return _buildDayItem(
          DateFormat('EEE').format(dateToDisplay), // "Mon", "Tue", etc.
          dateToDisplay.day.toString(),
          isSelected,
          isSpecial,
          dateToDisplay,
        );
      }),
    );
  }

  // Helper widget for a single day item in the picker
  Widget _buildDayItem(String day, String date, bool isSelected, bool isSpecial, DateTime fullDate) {
    return GestureDetector(
      onTap: () => _onDaySelected(fullDate),
      child: Column(
        children: [
          Text(
            day,
            style: TextStyle(color: Colors.grey[400], fontSize: 13),
          ),
          SizedBox(height: 4),
          Container(
            padding: EdgeInsets.symmetric(horizontal: 8, vertical: 4),
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
                color: isSelected ? Colors.blue : (isSpecial ? Colors.orange : Colors.white),
              ),
            ),
          ),
        ],
      ),
    );
  }

  // Builds the card for displaying a single event
  Widget _buildEventCard(Event event, Function(Event) onToggleStatus) {
    return Card(
      margin: EdgeInsets.symmetric(vertical: 8.0),
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
                    style: TextStyle(
                      fontWeight: FontWeight.bold,
                      fontSize: 18,
                      color: Colors.white,
                    ),
                  ),
                ),
                Text(
                  event.time,
                  style: TextStyle(
                    fontSize: 16,
                    color: Colors.white70,
                  ),
                ),
              ],
            ),
            SizedBox(height: 8),
            Text(
              event.description,
              style: TextStyle(
                fontSize: 14,
                color: Colors.white70,
              ),
            ),
            SizedBox(height: 10),
            Align(
              alignment: Alignment.bottomRight,
              child: ElevatedButton(
                onPressed: () => onToggleStatus(event),
                style: ElevatedButton.styleFrom(
                  backgroundColor: event.isAnnounced ? Colors.orange : Colors.blue,
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(8),
                  ),
                ),
                child: Text(
                  event.isAnnounced ? 'Move to Wait' : 'Announce',
                  style: TextStyle(color: Colors.white),
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
    // Filter all events to get only the ones for the currently selected date
    final List<Event> dailyEvents = _allEvents.where((event) {
      return event.date.year == _currentDate.year &&
             event.date.month == _currentDate.month &&
             event.date.day == _currentDate.day;
    }).toList();

    // Separate the daily events into two lists
    final List<Event> announcedEvents = dailyEvents.where((event) => event.isAnnounced).toList();
    final List<Event> waitingEvents = dailyEvents.where((event) => !event.isAnnounced).toList();

    return Scaffold(
      backgroundColor: const Color(0xFF162D41),
      appBar: AppBar(
        leading: IconButton(
          icon: const Icon(Icons.navigate_before, color: Colors.white),
          onPressed: () => Navigator.pop(context),
        ),
        title: Text(
          'Reminders',
          style: TextStyle(color: Colors.white),
        ),
        backgroundColor: Colors.transparent, // Fully transparent
        elevation: 0,
      ),
      body: Padding(
        padding: const EdgeInsets.all(20.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              DateFormat('MMMM d, yyyy').format(_currentDate), // "July 6, 2025"
              style: TextStyle(color: Colors.white70, fontSize: 16),
            ),
            SizedBox(height: 8),
            Text(
              DateFormat('EEEE').format(_currentDate), // "Sunday"
              style: TextStyle(
                  fontWeight: FontWeight.bold, fontSize: 32, color: Colors.white),
            ),
            SizedBox(height: 20),
            
            _buildDayPickerRow(),

            SizedBox(height: 30),
            
            Text(
              'Announced',
              style: TextStyle(
                  fontWeight: FontWeight.bold, fontSize: 22, color: Colors.white),
            ),
            SizedBox(height: 10),
            Expanded(
              child: announcedEvents.isEmpty
                  ? Center(
                      child: Text(
                        'No announced events for this day.',
                        style: TextStyle(fontSize: 16, color: Colors.white54),
                      ),
                    )
                  : ListView.builder(
                      itemCount: announcedEvents.length,
                      itemBuilder: (context, index) {
                        return _buildEventCard(announcedEvents[index], _toggleAnnouncedStatus);
                      },
                    ),
            ),
            SizedBox(height: 20),
            
            Text(
              'Wait to Announce',
              style: TextStyle(
                  fontWeight: FontWeight.bold, fontSize: 22, color: Colors.white),
            ),
            SizedBox(height: 10),
            Expanded(
              child: waitingEvents.isEmpty
                  ? Center(
                      child: Text(
                        'No events waiting for this day.',
                        style: TextStyle(fontSize: 16, color: Colors.white54),
                      ),
                    )
                  : ListView.builder(
                      itemCount: waitingEvents.length,
                      itemBuilder: (context, index) {
                        return _buildEventCard(waitingEvents[index], _toggleAnnouncedStatus);
                      },
                    ),
            ),
          ],
        ),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: () async {
          // Navigate to the AddEventScreen and wait for a result
          final newEvent = await Navigator.of(context).push<Event>(
            MaterialPageRoute(builder: (context) => AddEventScreen()),
          );
          // If the user saved an event (newEvent is not null), add it to the list
          if (newEvent != null) {
            _addEvent(newEvent);
          }
        },
        backgroundColor: Color(0xFFFDCD8B),
        child: Icon(Icons.add),
      ),
    );
  }
}

class AddEventScreen extends StatefulWidget {
  const AddEventScreen({super.key});

  @override
  State<AddEventScreen> createState() => _AddEventScreenState();
}

class _AddEventScreenState extends State<AddEventScreen> {

  final _formKey = GlobalKey<FormState>();
  String _title = '';
  String _description = '';
  DateTime _selectedDate = DateTime.now();
  TimeOfDay _selectedTime = TimeOfDay.now(); // Reverted to TimeOfDay for picker

  // Function to pick a date
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
              primary: Color(0xFFFDCD8B), // A yellow/gold for accents
              onPrimary: Colors.black, // Text color on primary
              surface: Color(0xFF162D41), // Background of the picker
              onSurface: Colors.white, // Text color on surface
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

  // Function to pick a time using numeric input mode
  Future<void> _pickTime() async {
    final TimeOfDay? picked = await showTimePicker(
      context: context,
      initialTime: _selectedTime,
      initialEntryMode: TimePickerEntryMode.input, // THIS IS THE KEY CHANGE
      builder: (context, child) {
        return Theme(
          data: ThemeData.dark().copyWith( // Apply dark theme to time picker
            colorScheme: const ColorScheme.dark(
              primary: Color(0xFFFDCD8B), // A yellow/gold for accents
              onPrimary: Colors.black, // Text color on primary
              surface: Color(0xFF162D41), // Background of the picker
              onSurface: Colors.white, // Text color on surface
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
              // Title Input
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

              // Description Input
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

              // Date Picker
              ListTile(
                title: Text(
                  'Date: ${DateFormat('yyyy-MM-dd').format(_selectedDate)}', // Display only date
                  style: const TextStyle(color: Colors.white),
                ),
                trailing: const Icon(Icons.calendar_today, color: Color(0xFFFDCD8B)), // Gold calendar icon
                onTap: _pickDate,
              ),
              const SizedBox(height: 8.0),

              // Time Picker (using numeric input mode)
              ListTile(
                title: Text(
                  'Time: ${_selectedTime.format(context)}',
                  style: const TextStyle(color: Colors.white),
                ),
                trailing: const Icon(Icons.access_time, color: Color(0xFFFDCD8B)), // Gold clock icon
                onTap: _pickTime, // Calls the modified _pickTime function
              ),
              const SizedBox(height: 32.0),

              // Save Event Button
              Center(
                child: ElevatedButton(
                  style: ElevatedButton.styleFrom(
                    backgroundColor: Color(0xFFFDCD8B), // Gold background for the button
                    foregroundColor: Colors.black, // Black text on gold button
                    padding: const EdgeInsets.symmetric(horizontal: 40, vertical: 15),
                    shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(30.0), // Rounded corners
                    ),
                    elevation: 5, // Slightly raised button
                  ),
                  onPressed: () {
                    if (_formKey.currentState!.validate()) {
                      _formKey.currentState!.save();
                      final newEvent = Event(
                        title: _title,
                        description: _description,
                        date: _selectedDate,
                        time: _selectedTime.format(context), // Use formatted TimeOfDay
                        isAnnounced: false, // Default
                      );
                      Navigator.of(context).pop(newEvent);
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
