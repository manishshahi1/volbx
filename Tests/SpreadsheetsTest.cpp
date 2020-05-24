#include "SpreadsheetsTest.h"

#include <memory>

#include <QApplication>
#include <QClipboard>
#include <QString>
#include <QTableView>
#include <QtTest/QtTest>

#include "Common/Configuration.h"
#include "Common/Constants.h"
#include "Common/ExportUtilities.h"
#include "Datasets/Dataset.h"
#include "Datasets/DatasetOds.h"
#include "Datasets/DatasetXlsx.h"
#include "ModelsAndViews/FilteringProxyModel.h"
#include "ModelsAndViews/TableModel.h"

#include "Common.h"

void SpreadsheetsTest::initTestCase()
{
    // generateAllDumpData();
}

QString SpreadsheetsTest::getSpreadsheetsDir()
{
    return QString(QApplication::applicationDirPath() +
                   QLatin1String("/TestSpreadsheets/"));
}

void SpreadsheetsTest::detailedSpreadsheetFileTest01()
{
    QString file(getSpreadsheetsDir() + "detailedTests/test01.xlsx");
    DatasetXlsx* definitionXlsx = new DatasetXlsx(file, file);
    testSpreadsheetFile01(definitionXlsx, file);
    file = getSpreadsheetsDir() + "detailedTests/test01Partial.xlsx";
    definitionXlsx = new DatasetXlsx(file, file);
    testSpreadsheetFile01SomeColumns(definitionXlsx);

    file = getSpreadsheetsDir() + "detailedTests/test01.ods";
    DatasetOds* definitionOds = new DatasetOds(file, file);
    testSpreadsheetFile01(definitionOds, file);
    file = getSpreadsheetsDir() + "detailedTests/test01Partial.ods";
    definitionOds = new DatasetOds(file, file);
    testSpreadsheetFile01SomeColumns(definitionOds);
}

// void SpreadsheetsTest::detailedSpreadsheetFileTest02()
//{
//    QString file(getSpreadsheetsDir() + "detailedTests/test02.xlsx");
//    DatasetXlsx* definitionXlsx = new DatasetXlsx(file,
//    file); testSpreadsheetFile02(definitionXlsx, file);

//    file = getSpreadsheetsDir() + "detailedTests/test02.ods";
//    DatasetOds* definitionOds = new DatasetOds(file,
//    file); testSpreadsheetFile02(definitionOds, file);
//}

void SpreadsheetsTest::detailedSpreadsheetFileTest03()
{
    QString file(getSpreadsheetsDir() + "detailedTests/test03.xlsx");
    DatasetXlsx* definitionXlsx = new DatasetXlsx(file, file);
    testSpreadsheetFile03(definitionXlsx, file);

    file = getSpreadsheetsDir() + "detailedTests/test03.ods";
    DatasetOds* definitionOds = new DatasetOds(file, file);
    testSpreadsheetFile03(definitionOds, file);
}

void SpreadsheetsTest::detailedSpreadsheetFileTest04()
{
    QString file(getSpreadsheetsDir() + "detailedTests/test04.xlsx");
    DatasetXlsx* definitionXlsx = new DatasetXlsx(file, file);
    testSpreadsheetFile04(definitionXlsx, file);

    file = getSpreadsheetsDir() + "detailedTests/test04.ods";
    DatasetOds* definitionOds = new DatasetOds(file, file);
    testSpreadsheetFile04(definitionOds, file);
}

// void SpreadsheetsTest::detailedSpreadsheetFileTest05()
//{
//    QString file(getSpreadsheetsDir() + "detailedTests/test05.xlsx");
//    DatasetXlsx* definitionXlsx = new DatasetXlsx(file,
//    file); testSpreadsheetFile05(definitionXlsx, file);

//    file = getSpreadsheetsDir() + "detailedTests/test05.ods";
//    DatasetOds* definitionOds = new DatasetOds(file,
//    file); testSpreadsheetFile05(definitionOds, file);
//}

void SpreadsheetsTest::testSpreadsheetFile01(DatasetSpreadsheet* dataset,
                                             QString file)
{
    QVERIFY(true == dataset->initialize());
    testBasicInfo(*dataset, 4000, 7, file);

    QVector<QPair<int, ColumnType> > columnFormats;
    columnFormats.append(qMakePair(0, ColumnType::STRING));
    columnFormats.append(qMakePair(3, ColumnType::NUMBER));
    columnFormats.append(qMakePair(5, ColumnType::NUMBER));
    QVector<QPair<int, QString> > columnNames;
    columnNames.append(qMakePair(0, QString("Trait #1")));
    columnNames.append(qMakePair(3, QString("Units")));
    columnNames.append(qMakePair(5, QString("Price per unit")));
    testColumnInfo(*dataset, columnFormats, columnNames);

    QVector<std::tuple<QVariant, int, int> > fields;
    fields.append(
        std::make_tuple(QVariant(QDate::fromJulianDay(2455207)), 3, 2));
    fields.append(std::make_tuple(QVariant("black"), 3, 6));
    fields.append(std::make_tuple(QVariant(12.0), 5, 1));
    fields.append(std::make_tuple(QVariant(4462.2), 3, 5));
    testSampleData(*dataset, 10, 7, fields);

    QVector<bool> activeColumns(dataset->columnCount(), true);
    dataset->setActiveColumns(activeColumns);

    QVector<int> columnsToTest;
    columnsToTest << 5 << 3 << 2 << 6 << 2 << 5;

    QVector<double> compareNumericValues;
    compareNumericValues << 803.25 << 39999.98 << 14.91 << 126.69;

    QVector<QDate> compareDateValues;
    compareDateValues << QDate(2010, 1, 1) << QDate(2011, 8, 31);

    QStringList compareList;
    compareList << "brown"
                << "red"
                << "yellow"
                << "black"
                << "blue"
                << "pink"
                << "white"
                << "dark blue"
                << "orange";

    dataset->loadData();

    // Special column after rebuild done in dataset init.
    dataset->setSpecialColumn(SpecialColumn::TRANSACTION_DATE, 2);
    dataset->setSpecialColumn(SpecialColumn::PRICE_PER_UNIT, 5);

    testDatasetConstruction(*dataset, columnsToTest, compareNumericValues,
                            compareDateValues, compareList, false);

    compareExportDataWithDump((std::unique_ptr<Dataset>(dataset)));
}

void SpreadsheetsTest::testSpreadsheetFile01SomeColumns(
    DatasetSpreadsheet* dataset)
{
    QVERIFY(true == dataset->initialize());
    QVector<bool> activeColumns(dataset->columnCount(), true);
    activeColumns[0] = false;
    activeColumns[1] = false;
    activeColumns[4] = false;
    dataset->setActiveColumns(activeColumns);

    QVector<int> columnsToTest;
    columnsToTest << 2 << 1 << 0 << 3 << 0 << 2;

    QVector<double> compareNumericValues;
    compareNumericValues << 803.25 << 39999.98 << 14.91 << 126.69;

    QVector<QDate> compareDateValues;
    compareDateValues << QDate(2010, 1, 1) << QDate(2011, 8, 31);

    QStringList compareList;
    compareList << "brown"
                << "red"
                << "yellow"
                << "black"
                << "blue"
                << "pink"
                << "white"
                << "dark blue"
                << "orange";

    dataset->loadData();
    dataset->setSpecialColumn(SpecialColumn::TRANSACTION_DATE, 0);
    dataset->setSpecialColumn(SpecialColumn::PRICE_PER_UNIT, 2);

    testDatasetConstruction(*dataset, columnsToTest, compareNumericValues,
                            compareDateValues, compareList, false);

    compareExportDataWithDump((std::unique_ptr<Dataset>(dataset)));  //"partial"
}

// void SpreadsheetsTest::testSpreadsheetFile02(DatasetDefinitionSpreadsheet*
// definition,
//                                         QString file)
//{
//    QVERIFY(true == definition->init());
//}

void SpreadsheetsTest::testSpreadsheetFile03(DatasetSpreadsheet* dataset,
                                             QString file)
{
    QVERIFY(true == dataset->initialize());
    testBasicInfo(*dataset, 4, 5, file);

    QVector<QPair<int, ColumnType> > columnFormats;
    columnFormats.append(qMakePair(0, ColumnType::NUMBER));
    columnFormats.append(qMakePair(1, ColumnType::NUMBER));
    columnFormats.append(qMakePair(2, ColumnType::NUMBER));
    QVector<QPair<int, QString> > columnNames;
    columnNames.append(qMakePair(0, QString("cena nier")));
    columnNames.append(qMakePair(1, QString("pow")));
    columnNames.append(qMakePair(3, QString("data transakcji")));
    testColumnInfo(*dataset, columnFormats, columnNames);

    QVector<std::tuple<QVariant, int, int> > fields;
    fields.append(std::make_tuple(QVariant(3703.75925925926), 3, 2));
    fields.append(std::make_tuple(QVariant(53.0), 2, 1));
    fields.append(std::make_tuple(QVariant(3773.62264150943), 2, 2));
    testSampleData(*dataset, 4, 5, fields);

    QVector<bool> activeColumns(dataset->columnCount(), true);
    dataset->setActiveColumns(activeColumns);

    QVector<int> columnsToTest;
    columnsToTest << 0 << 1 << 3 << 4 << 3 << 2;

    QVector<double> compareNumericValues;
    compareNumericValues << 200000 << 200003 << 51 << 54;

    QVector<QDate> compareDateValues;
    compareDateValues << QDate(2012, 2, 2) << QDate(2012, 2, 5);

    QStringList compareList;
    compareList << "a"
                << "b"
                << "c";

    dataset->loadData();
    dataset->setSpecialColumn(SpecialColumn::TRANSACTION_DATE, 3);
    dataset->setSpecialColumn(SpecialColumn::PRICE_PER_UNIT, 2);

    testDatasetConstruction(*dataset, columnsToTest, compareNumericValues,
                            compareDateValues, compareList, true);

    compareExportDataWithDump((std::unique_ptr<Dataset>(dataset)));
}

void SpreadsheetsTest::testSpreadsheetFile04(DatasetSpreadsheet* dataset,
                                             QString file)
{
    QVERIFY(true == dataset->initialize());
    testBasicInfo(*dataset, 30, 12, file);

    QVector<QPair<int, ColumnType> > columnFormats;
    columnFormats.append(qMakePair(0, ColumnType::STRING));
    columnFormats.append(qMakePair(1, ColumnType::DATE));
    columnFormats.append(qMakePair(2, ColumnType::NUMBER));
    columnFormats.append(qMakePair(3, ColumnType::NUMBER));
    columnFormats.append(qMakePair(4, ColumnType::DATE));
    columnFormats.append(qMakePair(5, ColumnType::NUMBER));
    columnFormats.append(qMakePair(6, ColumnType::STRING));
    columnFormats.append(qMakePair(7, ColumnType::STRING));
    columnFormats.append(qMakePair(8, ColumnType::NUMBER));
    columnFormats.append(qMakePair(9, ColumnType::NUMBER));
    columnFormats.append(qMakePair(10, ColumnType::NUMBER));
    columnFormats.append(qMakePair(11, ColumnType::NUMBER));

    QVector<QPair<int, QString> > columnNames;
    columnNames.append(qMakePair(0, QString("name")));
    columnNames.append(qMakePair(1, QString("date")));
    columnNames.append(qMakePair(2, QString("mass (kg)")));
    columnNames.append(qMakePair(3, QString("height")));
    columnNames.append(qMakePair(4, QString("no name")));
    columnNames.append(qMakePair(5, QString("no name")));
    columnNames.append(qMakePair(6, QString("no name")));
    columnNames.append(qMakePair(7, QString("no name")));
    columnNames.append(qMakePair(8, QString("no name")));
    columnNames.append(qMakePair(9, QString("no name")));
    columnNames.append(qMakePair(10, QString("no name")));
    columnNames.append(qMakePair(11, QString("no name")));
    testColumnInfo(*dataset, columnFormats, columnNames);

    QVector<std::tuple<QVariant, int, int> > fields;
    fields.append(std::make_tuple(QVariant(1.55), 3, 3));
    fields.append(std::make_tuple(QVariant(58.57), 5, 2));
    testSampleData(*dataset, 10, 12, fields);

    QVector<bool> activeColumns(dataset->columnCount(), true);
    dataset->setActiveColumns(activeColumns);

    QVector<int> columnsToTest;
    columnsToTest << 2 << 3 << 1 << 0 << 1 << 2;

    QVector<double> compareNumericValues;
    compareNumericValues << 0 << 74.46 << 0 << 1.83;

    QVector<QDate> compareDateValues;
    compareDateValues << QDate(1970, 1, 1) << QDate(1970, 1, 30);

    QStringList compareList;

    dataset->loadData();
    dataset->setSpecialColumn(SpecialColumn::TRANSACTION_DATE, 1);
    dataset->setSpecialColumn(SpecialColumn::PRICE_PER_UNIT, 2);

    testDatasetConstruction(*dataset, columnsToTest, compareNumericValues,
                            compareDateValues, compareList, false);

    compareExportDataWithDump((std::unique_ptr<Dataset>(dataset)));
}

// void
// SpreadsheetsTest::testSpreadsheetFile05(DatasetDefinitionSpreadsheet*
// definition,
//                                             QString file)
//{
//    QVERIFY(true == definition->init());
//}

void SpreadsheetsTest::testBasicInfo(Dataset& dataset, int rows, int columns,
                                     QString name)
{
    QVERIFY(dataset.isValid());
    QCOMPARE(dataset.rowCount(), rows);
    QCOMPARE(dataset.columnCount(), columns);

    QString dump = Common::loadFile(
        QString(dataset.getName() + Common::getDefinitionDumpSuffix()));
    QCOMPARE(dataset.dumpDatasetDefinition(), dump);

    QCOMPARE(dataset.getName(), name);
}

void SpreadsheetsTest::testColumnInfo(
    Dataset& dataset, const QVector<QPair<int, ColumnType> >& columnFormats,
    const QVector<QPair<int, QString> >& columnNames)
{
    QPair<int, ColumnType> pairFormat;
    foreach (pairFormat, columnFormats)
    {
        QCOMPARE(dataset.getColumnFormat(pairFormat.first), pairFormat.second);
    }

    // No special columns yet.
    auto [ok, column] =
        dataset.getSpecialColumn(SpecialColumn::TRANSACTION_DATE);
    QVERIFY(!ok);
    std::tie(ok, column) =
        dataset.getSpecialColumn(SpecialColumn::PRICE_PER_UNIT);
    QVERIFY(!ok);

    QPair<int, QString> pairNames;
    foreach (pairNames, columnNames)
    {
        QCOMPARE(dataset.getHeaderName(pairNames.first), pairNames.second);
    }
}

void SpreadsheetsTest::testSampleData(
    Dataset& dataset, int rows, int columns,
    const QVector<std::tuple<QVariant, int, int> >& fields)
{
    QVector<QVector<QVariant> > sampleData{dataset.retrieveSampleData()};

    QCOMPARE(sampleData.size(), rows);
    QCOMPARE(sampleData.front().size(), columns);

    std::tuple<QVariant, int, int> fieldsTuple;
    for (auto [value, row, column] : fields)
        QCOMPARE(sampleData.at(row)[column], value);
}

void SpreadsheetsTest::testDatasetConstruction(
    const Dataset& dataset, QVector<int>& columnsToTest,
    QVector<double>& compareNumericValues, QVector<QDate>& compareDateValues,
    QStringList& compareList, bool emptyDates)
{
    QVERIFY(dataset.isValid());

    auto [min, max] = dataset.getNumericRange(columnsToTest[0]);
    QCOMPARE(min, compareNumericValues[0]);
    QCOMPARE(max, compareNumericValues[1]);

    std::tie(min, max) = dataset.getNumericRange(columnsToTest[1]);
    QCOMPARE(min, compareNumericValues[2]);
    QCOMPARE(max, compareNumericValues[3]);

    auto [minDate, maxDate, empty] = dataset.getDateRange(columnsToTest[2]);
    QCOMPARE(minDate, compareDateValues[0]);
    QCOMPARE(maxDate, compareDateValues[1]);
    QCOMPARE(empty, emptyDates);

    QStringList list = dataset.getStringList(columnsToTest[3]);
    QCOMPARE(list, compareList);

    auto [ok, column] =
        dataset.getSpecialColumn(SpecialColumn::TRANSACTION_DATE);
    QCOMPARE(column, columnsToTest[4]);
    std::tie(ok, column) =
        dataset.getSpecialColumn(SpecialColumn::PRICE_PER_UNIT);
    QCOMPARE(column, columnsToTest[5]);
}

void SpreadsheetsTest::compareDataWithDumps(const QString& category,
                                            bool damaged)
{
    QDir dir(getSpreadsheetsDir() + category);
    QStringList nameFilter;
    nameFilter << "*.xlsx"
               << "*.ods";
    QFileInfoList list = dir.entryInfoList(nameFilter, QDir::Files);

    foreach (QFileInfo f, list)
    {
        QString fileName(f.filePath());
        DatasetSpreadsheet* dataset = nullptr;
        if (fileName.endsWith(".xlsx"))
            dataset = new DatasetXlsx(fileName, fileName);
        else
            dataset = new DatasetOds(fileName, fileName);

        if (true == damaged)
        {
            // Damaged files test.
            QVERIFY(false == dataset->initialize());
        }
        else
        {
            QVERIFY(true == dataset->initialize());
            QString dump = Common::loadFile(
                QString(fileName + Common::getDefinitionDumpSuffix()));
            QCOMPARE(dataset->dumpDatasetDefinition(), dump);

            QVector<bool> activeColumns(dataset->columnCount(), true);
            dataset->setActiveColumns(activeColumns);
            dataset->loadData();
            QVERIFY(true == dataset->isValid());

            compareExportDataWithDump((std::unique_ptr<Dataset>(dataset)));
        }
    }
}

void SpreadsheetsTest::compareAllDefinitionDumps()
{
    QDirIterator dirIt(getSpreadsheetsDir(), QDirIterator::Subdirectories);
    while (dirIt.hasNext())
    {
        dirIt.next();
        QFileInfo fileInfo(dirIt.filePath());
        if (fileInfo.isFile() &&
            (fileInfo.suffix() == "xlsx" || fileInfo.suffix() == "ods"))
        {
            QCOMPARE(
                Common::loadFile(fileInfo.path() + "/" + fileInfo.baseName() +
                                 ".xlsx" + Common::getDefinitionDumpSuffix()),
                Common::loadFile(fileInfo.path() + "/" + fileInfo.baseName() +
                                 ".ods" + Common::getDefinitionDumpSuffix()));
        }
    }
}

void SpreadsheetsTest::compareAllTsvDumps()
{
    QDirIterator dirIt(getSpreadsheetsDir(), QDirIterator::Subdirectories);
    while (dirIt.hasNext())
    {
        dirIt.next();
        QFileInfo fileInfo(dirIt.filePath());
        if (fileInfo.isFile() &&
            (fileInfo.suffix() == "xlsx" || fileInfo.suffix() == "ods"))
        {
            QCOMPARE(
                Common::loadFile(fileInfo.path() + "/" + fileInfo.baseName() +
                                 ".xlsx" + Common::getDataTsvDumpSuffix()),
                Common::loadFile(fileInfo.path() + "/" + fileInfo.baseName() +
                                 ".ods" + Common::getDataTsvDumpSuffix()));
        }
    }
}

void SpreadsheetsTest::cleanupTestCase() {}

void SpreadsheetsTest::generateAllDumpData()
{
    QDirIterator dirIt(getSpreadsheetsDir(), QDirIterator::Subdirectories);
    while (dirIt.hasNext())
    {
        dirIt.next();
        QFileInfo fileInfo(dirIt.filePath());
        if (fileInfo.isFile() &&
            (fileInfo.suffix() == "xlsx" || fileInfo.suffix() == "ods"))
        {
            generateDataDumpsForFile(fileInfo.filePath());
        }
    }
}

void SpreadsheetsTest::compareExportDataWithDump(
    std::unique_ptr<Dataset> dataset)
{
    QString datasetName{dataset->getName()};
    TableModel model(std::move(dataset));
    FilteringProxyModel proxyModel;
    proxyModel.setSourceModel(&model);

    QTableView view;
    view.setModel(&proxyModel);

    QString actualData{Common::getExportedTsv(view)};
    QString expectedData{
        Common::loadFile(datasetName + Common::getDataTsvDumpSuffix())};
    QCOMPARE(actualData.split('\n'), expectedData.split('\n'));
}

void SpreadsheetsTest::generateDataDumpsForFile(QString name)
{
    DatasetSpreadsheet* dataset = nullptr;
    if (name.endsWith(".xlsx"))
    {
        dataset = new DatasetXlsx(name, name);
    }
    else
    {
        dataset = new DatasetOds(name, name);
    }

    dataset->initialize();
    QString dumpedDefinition = dataset->dumpDatasetDefinition();
    Common::saveFile(name + Common::getDefinitionDumpSuffix(),
                     dumpedDefinition);

    if (name.contains("damagedFiles"))
    {
        return;
    }

    QVector<bool> activeColumns(dataset->columnCount(), true);
    dataset->setActiveColumns(activeColumns);
    dataset->loadData();

    TableModel model((std::unique_ptr<Dataset>(dataset)));
    FilteringProxyModel proxyModel;
    proxyModel.setSourceModel(&model);

    QTableView view;
    view.setModel(&proxyModel);

    QString tsvData{Common::getExportedTsv(view)};
    Common::saveFile(name + Common::getDataTsvDumpSuffix(), tsvData);
}

void SpreadsheetsTest::verySmallFiles()
{
    compareDataWithDumps("verySmallFiles(max 20KB)", false);
}

void SpreadsheetsTest::smallFiles()
{
    compareDataWithDumps("smallFiles(21-100KB)", false);
}

void SpreadsheetsTest::mediumFiles()
{
    compareDataWithDumps("mediumFiles(101-250KB)", false);
}

void SpreadsheetsTest::bigFiles()
{
    compareDataWithDumps("bigFiles(251-1000KB)", false);
}

void SpreadsheetsTest::veryBigFiles()
{
    compareDataWithDumps("veryBigFiles(min 1MB)", false);
}

void SpreadsheetsTest::damaged() { compareDataWithDumps("damagedFiles", true); }
