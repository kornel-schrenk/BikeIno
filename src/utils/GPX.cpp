#include "utils/GPX.h"

GPX::GPX(){
}

//Get methods

String GPX::getOpen(){
  return String(_GPX_HEAD);
}

String GPX::getClose(){
  return String(_GPX_TAIL);
}

String GPX::getMetaData(){
  String localStr(_GPX_META_HEAD);
  if (_metaName.length() > 0){
    localStr = localStr + String(_GPX_NAME_HEAD);
    localStr = localStr + _metaName;
    localStr = localStr + String(_GPX_NAME_TAIL);
  }
  if (_metaDesc.length() > 0){
    localStr = localStr + String(_GPX_DESC_HEAD);
    localStr = localStr + _wrapCDATA(_metaDesc);
    localStr = localStr + String(_GPX_DESC_TAIL);
  }
  localStr = localStr + String(_GPX_META_TAIL);
  return localStr;
}

String GPX::getTrackOpen(){
  return String(_GPX_TRAK_HEAD);
}

String GPX::getTrackClose(){
  return String(_GPX_TRAK_TAIL);
}

String GPX::getTrackSegmentOpen(){
  return String(_GPX_TRKSEG_HEAD);
}

String GPX::getTrackSegmentClose(){
  return String(_GPX_TRKSEG_TAIL);
}

String GPX::getInfo(){
  String localStr("");
  if (_name.length() > 0){
    localStr += _GPX_NAME_HEAD;
    localStr += _name;
    localStr += _GPX_NAME_TAIL;
  }
  if (_desc.length() > 0){
    localStr += _GPX_DESC_HEAD;
    localStr += _desc;
    localStr += _GPX_DESC_TAIL;
  }
  return localStr;
}

String GPX::getPt(String typ, String lon, String lat){
  String localStr(_GPX_PT_HEAD);
  localStr.replace("TYPE",typ);
  localStr += lat + "\" lon=\""; 
  localStr += lon + "\">\n";
  if (_ele.length() > 0){
    localStr += _GPX_ELE_HEAD;
    localStr += _ele;
    localStr += _GPX_ELE_TAIL;
  }
  if (_sym.length() > 0){
    localStr += _GPX_SYM_HEAD;
    localStr += _sym;
    localStr += _GPX_SYM_TAIL;
  }
  if (_src.length() > 0){
    localStr += _GPX_SRC_HEAD;
    localStr += _src;
    localStr += _GPX_SRC_TAIL;
  }
  String tail = String(_GPX_PT_TAIL);
  tail.replace("TYPE",typ);
  localStr += tail;
  return localStr;
}
    
String GPX::getPt(String typ, String lon, String lat, String ele){
  this->_ele = ele;
  return this->getPt(typ, lon, lat);
}

//Set Methods
void GPX::setMetaName(String name){
  _metaName = name;
}
void GPX::setMetaDesc(String desc){
  _metaDesc = desc;
}
void GPX::setName(String name){
  _name = name;
}
void GPX::setDesc(String desc){
  _desc = desc;
}
void GPX::setEle(String ele){
  _ele = ele;
}
void GPX::setSym(String sym){
  _sym = sym;
}
void GPX::setSrc(String src){
  _src = src;
}

String GPX::_wrapCDATA(String input){
  String localStr("<![CDATA[");
  localStr += input;
  localStr += "]]>";

  return localStr;
}
