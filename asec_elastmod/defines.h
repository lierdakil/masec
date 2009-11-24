#ifndef DEFINES_H
#define DEFINES_H

#define getparam(p,u) \
{ \
  QRegExp r(QString("%1\\s*=\\s*([0-9eE.]+)\\s*([A-z]+)").arg(#p));\
  if(r.indexIn(line)>=0) if(r.capturedTexts().at(2).toLower()==#u) p=r.capturedTexts().at(1).toDouble(); }

#define checkparam(p,u) \
if(p<0) { p=QInputDialog::getDouble(0, \
                                  "asec_elastmod", \
                                  QString("Please enter value for %1 in %2")\
                                  .arg(#p).arg(#u),\
                                  0,-2147483647,2147483647,10); \
            *out<<"#"#p" = "<<p<<#u"\n"; }

#define data_append(var) data<<QString::number(var,'f')

#endif // DEFINES_H
