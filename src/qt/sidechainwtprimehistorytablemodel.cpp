// Copyright (c) 2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/sidechainwtprimehistorytablemodel.h>

#include <QTimer>

#include <qt/bitcoinunits.h>
#include <qt/clientmodel.h>
#include <qt/guiconstants.h>
#include <qt/optionsmodel.h>
#include <qt/walletmodel.h>

#include <sidechain.h>
#include <txdb.h>
#include <validation.h>

Q_DECLARE_METATYPE(WTPrimeHistoryTableObject)

SidechainWTPrimeHistoryTableModel::SidechainWTPrimeHistoryTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int SidechainWTPrimeHistoryTableModel::rowCount(const QModelIndex & /*parent*/) const
{
    return model.size();
}

int SidechainWTPrimeHistoryTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 4;
}

QVariant SidechainWTPrimeHistoryTableModel::data(const QModelIndex &index, int role) const
{
    if (!walletModel)
        return false;

    if (!index.isValid())
        return false;

    int row = index.row();
    int col = index.column();

    if (!model.at(row).canConvert<WTPrimeHistoryTableObject>())
        return QVariant();

    WTPrimeHistoryTableObject object = model.at(row).value<WTPrimeHistoryTableObject>();

    int unit = walletModel->getOptionsModel()->getDisplayUnit();

    switch (role) {
    case Qt::DisplayRole:
    {
        // Height
        if (col == 0) {
            return QString::number(object.height);
        }
        // Hash
        if (col == 1) {
            return object.hash;
        }
        // Total Withdrawn
        if (col == 2) {

            QString amount = BitcoinUnits::formatWithUnit(unit, object.amount, false,
                    BitcoinUnits::separatorAlways);
            return amount;
        }
        // Status
        if (col == 3) {
            return object.status;
        }
    }
    }
    return QVariant();
}

QVariant SidechainWTPrimeHistoryTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0:
                return QString("Sidechain block #");
            case 1:
                return QString("Hash");
            case 2:
                return QString("Amount");
            case 3:
                return QString("Status");
            }
        }
    }
    return QVariant();
}

void SidechainWTPrimeHistoryTableModel::UpdateModel()
{
    beginResetModel();
    model.clear();
    endResetModel();

    // Get all of the current WT^(s)
    std::vector<SidechainWTPrime> vWTPrime;
    vWTPrime = psidechaintree->GetWTPrimes(SIDECHAIN_TEST);

    if (vWTPrime.empty())
        return;

    // Sort WT^(s) by height
    SortWTPrimeByHeight(vWTPrime);

    // Add WT^(s) to model
    beginInsertRows(QModelIndex(), model.size(), model.size() + vWTPrime.size() - 1);
    for (const SidechainWTPrime& wt : vWTPrime) {
        WTPrimeHistoryTableObject object;

        // Insert new WT^ into table
        object.hash = QString::fromStdString(wt.wtPrime.GetHash().ToString());
        object.amount = CTransaction(wt.wtPrime).GetValueOut();
        object.status = QString::fromStdString(wt.GetStatusStr());
        object.height = wt.nHeight;
        model.append(QVariant::fromValue(object));
    }
    endInsertRows();
}

bool SidechainWTPrimeHistoryTableModel::GetWTPrimeInfoAtRow(int row, uint256& hash) const
{
    if (row >= model.size())
        return false;

    if (!model[row].canConvert<WTPrimeHistoryTableObject>())
        return false;

    WTPrimeHistoryTableObject object = model[row].value<WTPrimeHistoryTableObject>();

    hash = uint256S(object.hash.toStdString());

    return true;
}

void SidechainWTPrimeHistoryTableModel::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
}

void SidechainWTPrimeHistoryTableModel::setClientModel(ClientModel *model)
{
    this->clientModel = model;
    if (model)
    {
        connect(model, SIGNAL(numBlocksChanged(int, QDateTime, double, bool)),
                this, SLOT(UpdateModel()));
    }
}
