import 'os_enum.dart';

class EnvironmentInfo {
  static const String _tagOsEnum = 'os_name';
  static const String _tagOsVersion = 'os_version';
  static const String _tagBrand = 'brand';
  static const String _tagModelCode = 'model_code';
  static const String _tagModelName = 'model_name';

  final OsName osName;
  final String osVersion;
  final String brand;
  final String modelCode;
  final String modelName;

  EnvironmentInfo({
    required this.brand,
    required this.modelCode,
    required this.modelName,
    required this.osName,
    required this.osVersion,
  });

  EnvironmentInfo.fromJson(Map<String, dynamic> json)
      : this(
          osName: OsName.fromJson(json[_tagOsEnum] as String),
          osVersion: json[_tagOsVersion] as String,
          brand: json[_tagBrand] as String,
          modelCode: json[_tagModelCode] as String,
          modelName: json[_tagModelName] as String,
        );

  Map<String, dynamic> toJson() => {
        _tagBrand: brand,
        _tagModelCode: modelCode,
        _tagModelName: modelName,
        _tagOsEnum: osName,
        _tagOsVersion: osVersion,
      };
}
