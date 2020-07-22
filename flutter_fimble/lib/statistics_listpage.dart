import 'package:fl_chart/fl_chart.dart';
import 'package:flutter/material.dart';
import 'dart:math';
import 'dart:async';
import 'statistics_indicator.dart';

class StatisticsListPage extends StatefulWidget {
  StatisticsListPage({Key key, this.title, this.color}) : super(key: key);

  final String title;
  final MaterialColor color;

  @override
  _StatisticsListPageState createState() => _StatisticsListPageState();
}


class _StatisticsListPageState extends State<StatisticsListPage> {
  Timer timer;

  List<FlSpot> dataset = [
    FlSpot(12, 4),
  ];

  double doubleInRange(Random source, double start, double end) =>
      source.nextDouble() * (end - start) + start;

  @override
  void initState() {
    super.initState();
    timer = Timer.periodic(Duration(milliseconds: 1000), (Timer t) {
      List<FlSpot> newdataset = [];
      dataset.forEach((FlSpot element) {
        if (element.x <= 0) return;
        newdataset.add(FlSpot(
          element.x - 1,
          element.y,
        ));
      });

      dataset.clear();
      dataset.addAll(newdataset);

      var randomY = doubleInRange(new Random(), 2.75, 3.25);
      dataset.add(FlSpot(12, randomY));

      setState(() {});
    });
  }

  @override
  void dispose() {
    timer?.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
        backgroundColor: widget.color,
      ),
      body: Column(children: <Widget>[
        Text(
          "Temperature Control",
          style: TextStyle(
            height: 2.5,
            color: Color(COLOR_GRAPH_TEXT),
            fontSize: 24,
            fontWeight: FontWeight.bold,
          ),
        ),
        createGraphWidget(),

        Text(
          "Food Category",
          style: TextStyle(
            height: 2.5,
            color: Color(COLOR_GRAPH_TEXT),
            fontSize: 24,
            fontWeight: FontWeight.bold,
          ),
        ),
        createDonutWidget(),
      ]),
    );
  }

  static const int COLOR_GRAPH_BG = 0x00000000;
  static const int COLOR_GRAPH_GRID = 0xffcaccd1;
  static const int COLOR_GRAPH_TEXT = 0xff616f67;

  static const int COLOR_GRADIENT_TOP = 0xff47cf73;
  static const int COLOR_GRADIENT_BOTTOM = 0xff0ebeff;

  Widget createGraphWidget() {
    return AspectRatio(
      aspectRatio: 1.9,
      child: Container(
        decoration: const BoxDecoration(
            color: Color(COLOR_GRAPH_BG),
        ),
        child: Padding(
          padding: const EdgeInsets.only(right: 16, left: 16, top: 8, bottom: 8),
          child: LineChart(
            createGraphData(),
          ),
        ),
      ),
    );
  }

  LineChartData createGraphData() {
    List<Color> gradientColors = [
      const Color(COLOR_GRADIENT_TOP),
      const Color(COLOR_GRADIENT_BOTTOM),
    ];

    return LineChartData(
      borderData: FlBorderData(
        show: true,
        border: Border.all(color: const Color(COLOR_GRAPH_GRID), width: 1),
      ),
      gridData: FlGridData(
        show: true,
        drawVerticalLine: true,
        getDrawingHorizontalLine: (value) {
          return FlLine(
            color: const Color(COLOR_GRAPH_GRID),
            strokeWidth: 1,
          );
        },
        getDrawingVerticalLine: (value) {
          return FlLine(
            color: const Color(COLOR_GRAPH_GRID),
            strokeWidth: 1,
          );
        },
      ),
      titlesData: FlTitlesData(
        show: true,
        bottomTitles: SideTitles(showTitles: false),
        leftTitles: SideTitles(
          showTitles: true,
          textStyle: const TextStyle(
            color: Color(COLOR_GRAPH_TEXT),
            fontWeight: FontWeight.bold,
            fontSize: 16,
          ),
          getTitles: (value) {
            switch (value.toInt()) {
              case 0:
                return '0°C';
              case 3:
                return '5°C';
              case 6:
                return '10°C';
            }
            return '';
          },
          reservedSize: 30,
          margin: 6,
        ),
      ),

      minX: 0,
      maxX: 12,
      minY: 0,
      maxY: 6,

      lineBarsData: [
        LineChartBarData(
          spots: dataset,
          isCurved: true,
          colors: gradientColors,
          gradientFrom: const Offset(0, 0), // Top
          gradientTo: const Offset(2, 2), // Bottom
          barWidth: 4,
          isStrokeCapRound: true,
          dotData: FlDotData(show: false),
          belowBarData: BarAreaData(
            show: true,
            colors:
                gradientColors.map((color) => color.withOpacity(0.4)).toList(),
            gradientFrom: const Offset(0, 0), // Top
            gradientTo: const Offset(0, 1), // Bottom
          ),
        ),
      ],
    );
  }

  int touchedIndex;

  Widget createDonutWidget() {
    return Padding(
      padding: const EdgeInsets.only(right: 0, left: 0, top: 16, bottom: 16),
      child: AspectRatio(
        aspectRatio: 1.8,
        child: Row(
          children: <Widget>[
            PieChart(
              PieChartData(
                  pieTouchData: PieTouchData(touchCallback: (pieTouchResponse) {
                    setState(() {
                      if (pieTouchResponse.touchInput is FlLongPressEnd ||
                          pieTouchResponse.touchInput is FlPanEnd) {
                        touchedIndex = -1;
                      } else {
                        touchedIndex = pieTouchResponse.touchedSectionIndex;
                      }
                    });
                  }),
                  borderData: FlBorderData(
                    show: false,
                  ),
                  sectionsSpace: 0,
                  centerSpaceRadius: 55,
                  sections: showingSections(),
              ),
            ),
            Column(
              mainAxisSize: MainAxisSize.max,
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.start,
              children: const <Widget>[
                Indicator(
                  color: Color(0xff0293ee),
                  text: 'Beverages',
                  isSquare: true,
                ),
                SizedBox(
                  height: 8,
                ),
                Indicator(
                  color: Color(0xfff8b250),
                  text: 'Meat &\nPoultry',
                  isSquare: true,
                ),
                SizedBox(
                  height: 8,
                ),
                Indicator(
                  color: Color(0xff845bef),
                  text: 'Fruits',
                  isSquare: true,
                ),
                SizedBox(
                  height: 8,
                ),
                Indicator(
                  color: Color(0xff13d38e),
                  text: 'Vegetables',
                  isSquare: true,
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }

  List<PieChartSectionData> showingSections() {
    return List.generate(4, (i) {
      final isTouched = i == touchedIndex;
      final double fontSize = isTouched ? 30 : 20;
      final double radius = isTouched ? 65 : 50;
      switch (i) {
        case 0:
          return PieChartSectionData(
            color: const Color(0xff0293ee),
            value: 40,
            title: '40%',
            radius: radius,
            titleStyle: TextStyle(
                fontSize: fontSize, fontWeight: FontWeight.bold, color: const Color(0xffffffff)),
          );
        case 1:
          return PieChartSectionData(
            color: const Color(0xfff8b250),
            value: 30,
            title: '30%',
            radius: radius,
            titleStyle: TextStyle(
                fontSize: fontSize, fontWeight: FontWeight.bold, color: const Color(0xffffffff)),
          );
        case 2:
          return PieChartSectionData(
            color: const Color(0xff845bef),
            value: 15,
            title: '15%',
            radius: radius,
            titleStyle: TextStyle(
                fontSize: fontSize, fontWeight: FontWeight.bold, color: const Color(0xffffffff)),
          );
        case 3:
          return PieChartSectionData(
            color: const Color(0xff13d38e),
            value: 15,
            title: '15%',
            radius: radius,
            titleStyle: TextStyle(
                fontSize: fontSize, fontWeight: FontWeight.bold, color: const Color(0xffffffff)),
          );
        default:
          return null;
      }
    });
  }

}