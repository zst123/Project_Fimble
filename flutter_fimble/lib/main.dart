import 'package:flutter/material.dart';
import 'package:bottom_navy_bar/bottom_navy_bar.dart';
import 'package:flutter_fimble/inventory_listpage.dart';
import 'package:flutter_fimble/statistics_listpage.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        primarySwatch: Colors.red,
        // Dark theme
        brightness: Brightness.light,
        // This makes the visual density adapt to the platform that you run
        // the app on. For desktop platforms, the controls will be smaller and
        // closer together (more dense) than on mobile platforms.
        visualDensity: VisualDensity.adaptivePlatformDensity,
      ),
      home: MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  MyHomePage({Key key, this.title}) : super(key: key);

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> with TickerProviderStateMixin {

  TabController _tabController;
  int currentIndex = 0;

  void setTabIndex(int i) {
    setState(() {
      _tabController.index = i;
      currentIndex = i;
    });
  }
  @override
  void initState() {
    _tabController = new TabController(length: 3, vsync: this);
    _tabController.addListener(() => setTabIndex(_tabController.index));
    super.initState();
  }

  @override
  void dispose() {
    _tabController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    // This method is rerun every time setState is called, for instance as done
    // by the _incrementCounter method above.
    //
    // The Flutter framework has been optimized to make rerunning build methods
    // fast, so that you can just rebuild anything that needs updating rather
    // than having to individually change instances of widgets.

    return DefaultTabController(
        length: 3,
        child: Scaffold(
          body: TabBarView(
            controller: _tabController,
            children: [
              new Container(
                child: InventoryListPage(title: "Project Fimble", color: Colors.red),
              ),
              new Container(
                child: StatisticsListPage(title: "Inventory Statistics", color: Colors.green),
              ),
              new Container(
                child: Scaffold(
                  appBar: AppBar(
                    title: Text("Inventory Overview"),
                    backgroundColor: Colors.purple,
                  ),
                  body: Container(
                    alignment: Alignment.center,
                    child: Image(image: AssetImage('img/artboard2.png')),
                  ),
                ),
              ),
            ],
          ),
          bottomNavigationBar: BottomNavyBar(
            selectedIndex: currentIndex,
            showElevation: true,
            itemCornerRadius: 8,
            curve: Curves.easeOutCubic,
            onItemSelected: (index) => setTabIndex(index),
            items: [
              BottomNavyBarItem(
                icon: Icon(Icons.dashboard),
                title: Text('Inventory'),
                activeColor: Colors.red,
                textAlign: TextAlign.center,
              ),
              BottomNavyBarItem(
                icon: Icon(Icons.event_note),
                title: Text(
                  'Statistics',
                ),
                activeColor: Colors.green,
                textAlign: TextAlign.center,
              ),
              BottomNavyBarItem(
                icon: Icon(Icons.settings),
                title: Text('Settings'),
                activeColor: Colors.purple,
                textAlign: TextAlign.center,
              ),
            ],
          ),
        ));
  }
}
