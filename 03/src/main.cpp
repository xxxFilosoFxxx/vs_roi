#include <QCoreApplication>
#include <QtConcurrent>
#include <QStringList>
#include <QTextCodec>
#include <QString>

#include <ctime>
#include <vector>
#include <iostream>

int main ( int argc_p, char ** argv_p ) { 
   QCoreApplication app_l( argc_p, argv_p );
   static const auto * const codel_scl = QTextCodec::codecForName( "IBM 866" );
   std::cout << "\n ---------------------------------\n";
   QStringList argumentsList_l;
   const std::size_t argumetsAmount_cl = argc_p;
   for ( std::size_t i = 0; i < argumetsAmount_cl; ++i )
      argumentsList_l.push_back( codel_scl->toUnicode( argv_p[ i ] ) );
   for ( auto oneArg : argumentsList_l )
      std::cout << "\t" << codel_scl->fromUnicode( oneArg ).constData() << "\n";
   std::cout << " ---------------------------------";
   return 0;
}