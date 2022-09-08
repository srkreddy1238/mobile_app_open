import 'dart:io';

import 'package:flutter/services.dart';

import 'package:device_info/device_info.dart';
import 'package:device_marketing_names/device_marketing_names.dart';
import 'package:mlperfbench_common/data/environment/environment_info.dart';
import 'package:mlperfbench_common/data/environment/os_enum.dart';

class DeviceInfo {
  final String manufacturer;
  final String brand;
  final String modelCode;
  final String modelName;

  static late final String nativeLibraryPath;
  static late final DeviceInfo instance;

  DeviceInfo({
    required this.manufacturer,
    required this.brand,
    required this.modelCode,
    required this.modelName,
  });

  static Future<void> staticInit() async {
    DeviceInfo.nativeLibraryPath = await _makeNativeLibraryPath();
    DeviceInfo.instance = await createFromEnvironment();
  }

  static Future<DeviceInfo> createFromEnvironment() async {
    if (Platform.isIOS) {
      return _makeIosInfo();
    } else if (Platform.isWindows) {
      return _makeWindowsInfo();
    } else if (Platform.isAndroid) {
      return _makeAndroidInfo();
    } else {
      throw 'Could not define platform';
    }
  }

  static Future<String> _makeNativeLibraryPath() async {
    if (Platform.isAndroid) {
      return await _getNativeLibraryPath();
    } else {
      return '';
    }
  }

  static Future<DeviceInfo> _makeIosInfo() async {
    final deviceInfo = DeviceInfoPlugin();
    final iosInfo = await deviceInfo.iosInfo;
    final deviceNames = DeviceMarketingNames();

    return DeviceInfo(
      manufacturer: 'Apple',
      brand: 'Apple',
      modelCode: iosInfo.utsname.machine,
      modelName: deviceNames.getSingleNameFromModel(
          DeviceType.ios, iosInfo.utsname.machine),
    );
  }

  static Future<DeviceInfo> _makeAndroidInfo() async {
    final deviceInfo = await DeviceInfoPlugin().androidInfo;
    final deviceNames = DeviceMarketingNames();

    print('deviceInfo.board == ${deviceInfo.board}');
    print('deviceInfo.manufacturer == ${deviceInfo.manufacturer}');
    print('deviceInfo.model == ${deviceInfo.model}');
    print('deviceInfo.androidId == ${deviceInfo.androidId}');
    print('deviceInfo.brand == ${deviceInfo.brand}');
    print('deviceInfo.device == ${deviceInfo.device}');
    print('deviceInfo.display == ${deviceInfo.display}');
    print('deviceInfo.fingerprint == ${deviceInfo.fingerprint}');
    print('deviceInfo.hardware == ${deviceInfo.hardware}');
    print('deviceInfo.host == ${deviceInfo.host}');
    print('deviceInfo.id == ${deviceInfo.id}');
    print('deviceInfo.product == ${deviceInfo.product}');
    print('deviceInfo.supportedAbis == ${deviceInfo.supportedAbis}');
    print('deviceInfo.tags == ${deviceInfo.tags}');
    print('deviceInfo.type == ${deviceInfo.type}');
    print('deviceInfo.version == ${deviceInfo.version}');

    return DeviceInfo(
      manufacturer: deviceInfo.manufacturer,
      brand: deviceInfo.brand,
      modelCode: deviceInfo.model,
      modelName: deviceNames.getSingleNameFromModel(
          DeviceType.android, deviceInfo.model),
    );
  }

  static Future<DeviceInfo> _makeWindowsInfo() async {
    return DeviceInfo(
      manufacturer: 'Unknown',
      brand: 'Unknown',
      modelCode: 'Unknown PC',
      modelName: 'Unknown PC',
    );
  }

  static EnvironmentInfo get environmentInfo {
    return EnvironmentInfo(
      osName: OsName.fromJson(Platform.operatingSystem),
      osVersion: Platform.operatingSystemVersion,
      brand: DeviceInfo.instance.brand,
      modelCode: DeviceInfo.instance.modelCode,
      modelName: DeviceInfo.instance.modelName,
    );
  }
}

const _androidChannel = MethodChannel('org.mlcommons.mlperfbench/android');
Future<String> _getNativeLibraryPath() async {
  return await _androidChannel.invokeMethod('getNativeLibraryPath') as String;
}
