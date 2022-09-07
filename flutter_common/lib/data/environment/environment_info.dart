import 'os_enum.dart';

class EnvironmentInfo {
  static const String _tagOsEnum = 'os_name';
  static const String _tagOsVersion = 'os_version';
  static const String _tagBrand = 'brand';
  static const String _tagModel = 'model';

  final OsName osName;
  final String osVersion;
  final String brand;
  final String model;

  EnvironmentInfo({
    required this.brand,
    required this.model,
    required this.osName,
    required this.osVersion,
  });

  EnvironmentInfo.fromJson(Map<String, dynamic> json)
      : this(
          osName: OsName.fromJson(json[_tagOsEnum] as String),
          osVersion: json[_tagOsVersion] as String,
          brand: json[_tagBrand] as String,
          model: json[_tagModel] as String,
        );

  Map<String, dynamic> toJson() => {
        _tagBrand: brand,
        _tagModel: model,
        _tagOsEnum: osName,
        _tagOsVersion: osVersion,
      };
}
