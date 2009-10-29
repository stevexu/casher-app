#ifndef DEPTABLEMODEL_H
#define DEPTABLEMODEL_H

#include <QAbstractTableModel>
#include <qlist.h>
#include <qvector>

class QStringList;

typedef QVector<QString> TableRow;
typedef QList<TableRow> TableRecords;
typedef TableRow TableHeader;

class TableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	TableModel(QObject *parent = 0);
	TableModel(TableRecords& listOfDep, QWidget* parent = 0);

	int rowCount(const QModelIndex &parent /* = QModelIndex */) const;
	int columnCount(const QModelIndex &parent /* = QModelIndex */) const;
	QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role  = Qt::EditRole );
	bool insertRows(int row, int count, const QModelIndex &parent  = QModelIndex());
	bool removeRows(int row, int count, const QModelIndex &parent  = QModelIndex() );
	TableRecords getList();

	void setHeader(const QStringList& headers);

	~TableModel();

private:
	TableRecords m_listOfDep;
	TableHeader m_header;
};

#endif // DEPTABLEMODEL_H
