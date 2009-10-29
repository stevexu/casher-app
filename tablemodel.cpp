#include <QtCore>
#include "tablemodel.h"

TableModel::TableModel(QObject *parent)
	: QAbstractTableModel(parent)
{

}

TableModel::TableModel(TableRecords& listOfDep, QWidget* parent /* = 0 */)
{
	m_listOfDep = listOfDep;
}

TableModel::~TableModel()
{

}

void TableModel::setHeader(const QStringList& headers)
{
	foreach(QString header, headers)
	{
		m_header.append(header);
	}
}

int TableModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return m_listOfDep.size();
}

int TableModel::columnCount(const QModelIndex &parent ) const
{
	Q_UNUSED(parent);
	return m_header.size();
}

QVariant TableModel::data(const QModelIndex &index, int role ) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	if (index.row() >= m_listOfDep.size() || index.row() < 0)
	{
		return QVariant();
	}

	if (role == Qt::DisplayRole)
	{
		TableRow rowData = m_listOfDep.at(index.row());

		return rowData[index.column()];
	}

	return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
	if (role != Qt::DisplayRole)
	{
		return QVariant();
	}

	if (orientation == Qt::Horizontal)
	{
		if (m_header.size() > section)
		{
			return m_header[section];
		}
		else
		{
			return QVariant();
		}

	} else if (orientation == Qt::Vertical)
	{
		return QVariant(section + 1);
	}

	return QVariant();
}

bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
	Q_UNUSED(parent);
	beginInsertRows(QModelIndex(), row, row + count - 1);
	
	for (int i = 0; i < count; i++)
	{
		TableRow rowData;
		// prepare a blank row for insert
		for (int j = 0; j < m_header.size(); j++)
		{
			rowData.append("");
		}
		m_listOfDep.insert(row, rowData);
	}
	
	endInsertRows();
	return true;
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
	Q_UNUSED(parent);
	beginRemoveRows(QModelIndex(), row, row + count - 1);

	for (int i = 0; i < count; ++i)
	{
		m_listOfDep.removeAt(row);
	}

	endRemoveRows();
	return true;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid() && role == Qt::EditRole)
	{
		int row = index.row();

		//QPair<QString, QString> pair = m_listOfDep.value(row);
		TableRow rowData = m_listOfDep.value(row);
		rowData[index.column()] = value.toString();
#if 0
		if (index.column() == 0)
		{
			pair.first = value.toString();
		}
		else if (index.column() == 1)
		{
			pair.second = value.toString();
		}
#endif
		m_listOfDep.replace(row, rowData);
		emit(dataChanged(index, index));
	}

	return false;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return Qt::ItemIsEnabled;
	}

	return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled;
}

TableRecords TableModel::getList()
{
	return m_listOfDep;
}