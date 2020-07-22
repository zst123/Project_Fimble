import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'package:pull_to_refresh/pull_to_refresh.dart';
import 'dart:convert';
import 'inventory_popup.dart';
import 'inventory_item.dart';

class InventoryListPage extends StatefulWidget {
  InventoryListPage({Key key, this.title, this.color}) : super(key: key);

  final String title;
  final MaterialColor color;

  @override
  _InventoryListPageState createState() => _InventoryListPageState();
}

class _InventoryListPageState extends State<InventoryListPage> {
  static const _url = "https://6o55ed7zce.execute-api.ap-southeast-1.amazonaws.com/retrieve";

  List<InventoryItem> _list = [];

  RefreshController _refreshController =  RefreshController(initialRefresh: true);

  @override
  void initState() {
    super.initState();
  }

  Future<void> updateList() async {
    // Get contents from page
    http.Client _client = new http.Client();
    final req = await _client.get(Uri.parse(_url));
    final content = req.body;
    print(content);

    // Parse JSON to list
    var parsedJson = jsonDecode(content);
    print(parsedJson['Items']);

    // Create inventory items from the list
    List<InventoryItem> mylist = [];
    for (var item in parsedJson['Items']) {
      mylist.add(
        new InventoryItem(
          data_id: item['data'],
          timestamp: int.parse(item['time']),
          device_id: item['device'],
        ),
      );
    }
    // Sort by name
    mylist.sort((a, b) => a.name.compareTo(b.name));

    // Update to displayed list
    _list.clear();
    _list.addAll(mylist);
  }

  void _onRefresh() async {
    await updateList();
    _refreshController.refreshCompleted();
    setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    ListTile makeListTile(InventoryItem _item) => ListTile(
          contentPadding:
              EdgeInsets.symmetric(horizontal: 10.0, vertical: 10.0),
          leading: Container(
            padding: EdgeInsets.only(right: 5.0),
            decoration: new BoxDecoration(
                border: new Border(
                    right: new BorderSide(width: 1.0, color: Colors.black38))),
            child: _item.icon,
            constraints: BoxConstraints.tightFor(width: 80),
          ),
          title: Text(
            _item.name,
            style: TextStyle(color: widget.color, fontWeight: FontWeight.bold),
          ),

          subtitle: Column(children: <Widget>[
            Row(
              children: <Widget>[
                Expanded(
                  flex: 1,
                  child: Container(
                    // tag: 'hero',
                    child: LinearProgressIndicator(
                        backgroundColor: Color.fromRGBO(209, 224, 224, 0.9),
                        value: _item.health, //_list.time,
                        valueColor: AlwaysStoppedAnimation(Color.lerp(
                            Colors.red, Colors.green, _item.health))),
                  ),
                ),
                Expanded(
                  flex: 4,
                  child: Padding(
                      padding: EdgeInsets.only(left: 10.0),
                      child: Text(_item.readable_time,
                          style: TextStyle(color: Colors.black))),
                ),
              ],
            ),
            Padding(
                padding: EdgeInsets.symmetric(vertical: 10.0),
                child: Text(_item.description,
                    textAlign: TextAlign.left,
                    style: TextStyle(color: Colors.black),
                    overflow: TextOverflow.fade,
                    maxLines: 3,
                    softWrap: true,
                ),
            ),
          ]),
          trailing:
              Icon(Icons.keyboard_arrow_right, color: Colors.grey, size: 30.0),
          onTap: () {


            final popup = InventoryPopup(context);
            TemplateInventoryPopup instance = popup.instance;

            popup.frameIcon = _item.icon;
            popup.frameContent = PageView(
              children: <Widget>[
                Container(
                  child: Text(
                    "\n\n\n" +
                    "${_item.description}",
                    style: TextStyle(fontSize: 16),
                    textAlign: TextAlign.center,
                  ),
                ),
                Container(
                  child: SingleChildScrollView(
                    scrollDirection: Axis.vertical,
                    child: Container(
                      child:
                          Image(image: AssetImage('img/inventory/orange.png')),
                    ),
                  ),
                ),
                Container(
                  child: Container(
                    alignment: Alignment.bottomCenter,
                      child: Image(image: AssetImage('img/inventory/orange_2.png')),
                  ),
                ),
              ],
            );

            popup.show(
              title: _item.name,
              actions: [
                popup.button(
                  outline: false,
                  label: 'Close',
                  onPressed: Navigator.of(context).pop,
                ),
              ],
            );

            /*
            Navigator.push(
                context,
                MaterialPageRoute(
                    //builder: (context) => DetailPage(lesson: lesson)
                    ));
             */
          },
        );

    Card makeCard(InventoryItem _item) => Card(
          elevation: 8.0,
          margin: new EdgeInsets.symmetric(horizontal: 10.0, vertical: 10.0),
          child: Container(
            // decoration: BoxDecoration(color: Color.fromRGBO(64, 75, 96, .9)),
            child: makeListTile(_item),
          ),
        );

    final makeBody = Container(
      // decoration: BoxDecoration(color: Color.fromRGBO(58, 66, 86, 1.0)),
      child: ListView.builder(
        physics: NeverScrollableScrollPhysics(),
        scrollDirection: Axis.vertical,
        shrinkWrap: true,
        itemCount: _list.length,
        itemBuilder: (BuildContext context, int index) {
          return makeCard(_list[index]);
        },
      ),
    );

    return Scaffold(
      appBar: AppBar(
        // Here we take the value from the MyHomePage object that was created by
        // the App.build method, and use it to set our appbar title.
        title: Text(widget.title),
        backgroundColor: widget.color,
      ),
      body: SmartRefresher(
        enablePullDown: true,
        enablePullUp: false,
        header: WaterDropMaterialHeader(),
        controller: _refreshController,
        onRefresh: _onRefresh,
        child: makeBody,
      ),
    );
  }
}
