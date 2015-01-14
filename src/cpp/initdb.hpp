/* name: initdb.hpp
 * date: 2015-01-11
 * author: Jakub Olczyk (jakub.olczyk@openmailbox.org)
 * license: Beerware (rev 42)
 */

#ifndef JO_INITDB__HPP___
#define JO_INITDB__HPP___

#include <QSqlDatabase>
#include <QSqlQueryModel>

QSqlDatabase initdb();

QSqlQueryModel* get_table(QSqlDatabase* db);

#endif //JO_INITDB__HPP___
