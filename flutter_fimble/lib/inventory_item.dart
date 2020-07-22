import 'package:flutter/material.dart';
import 'package:time_formatter/time_formatter.dart';
import 'dart:math';
import 'images_lookup.dart';

class InventoryItem {
  // AWS contents
  String data_id;
  int timestamp;
  String device_id;

  // Lookup values
  String name;
  String description;
  String device_name;
  String readable_time;
  double health;
  Image icon;

  InventoryItem({this.data_id, this.timestamp, this.device_id}) {
    // Lookup data & device
    name = InventoryImages.dataMap[data_id][0];
    description = InventoryImages.dataMap[data_id][1];
    //device_name = InventoryImages.deviceMap[device_id];
    icon = Image.network(
      InventoryImages.dataMap[data_id][2],
        fit: BoxFit.cover,
    );

    // Human readable time
    readable_time = formatTime(timestamp);

    // Health goes down the longer it is inside (to minimum of 10%)
    int msec_max = Duration(days: 3).inMilliseconds;
    int msec_since = new DateTime.now().millisecondsSinceEpoch - timestamp;
    health = max(
        (msec_max - msec_since) / msec_max,
        0.10
    );
  }
}
