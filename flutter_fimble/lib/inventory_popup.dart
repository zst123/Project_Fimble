import 'package:flutter/material.dart';

//import 'package:flutter/cupertino.dart';
import 'package:flutter_beautiful_popup/main.dart';

// https://github.com/jaweii/Flutter_beautiful_popup/blob/master/lib/templates/Gift.dart

class InventoryPopup extends BeautifulPopup {
  Widget frameIcon = Text('Hi');
  Widget frameContent = Text('Hi');

  InventoryPopup(context) : super(context: context);

  @override
  BeautifulPopupTemplate get instance {
    var template = super.instance;
    return template ?? TemplateInventoryPopup(this);
  }
}

class TemplateInventoryPopup extends TemplateGift {
  final BeautifulPopup options;
  TemplateInventoryPopup(this.options) : super(options);

  @override
  final illustrationKey = 'img/my_illustration.png';

  @override
  Color get primaryColor => options.primaryColor ?? Color(0xffFF2F49);
  @override
  final maxWidth = 500;
  @override
  final maxHeight = 640;
  @override
  final bodyMargin = 30;

  @override
  // TODO: implement button
  get button => super.button;

  @override
  get layout {
    final frameIcon = CircleAvatar(
      child: ClipOval(
          child: Container(
            child: (options as InventoryPopup).frameIcon,
            constraints: BoxConstraints(
              maxHeight: 120,
              maxWidth: 120,
              minHeight: 100,
              minWidth: 100,
            ),
          ),
      ),
      radius: 50,
      backgroundColor: Colors.white70,
    );

    final frameContent = (options as InventoryPopup).frameContent;

    return [
      Positioned(
        child: background,
      ),

      /* Top Icon */
      Positioned.fill(
        child: Align(
          alignment: Alignment.topCenter,
          child: frameIcon,
        ),
      ),

      /* Top Title */
      Positioned.fill(
        top: percentH(20),
        child: Align(
          alignment: Alignment.topCenter,
          child: title,
        ),
      ),

      Positioned(
        top: percentH(20),
        left: percentW(5),
        right: percentW(5),
        bottom: percentH(15),
        child: frameContent,
      ),

      /* Bottom Buttons */
      Positioned(
        bottom: percentW(5),
        left: percentW(5),
        right: percentW(5),
        child: actions ?? Container(),
      ),


    ];
  }
}
