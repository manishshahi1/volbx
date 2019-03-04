#ifndef DATASETSLISTBROWSER_H
#define DATASETSLISTBROWSER_H

#include <QWidget>

class QListWidgetItem;

namespace Ui
{
class DatasetsListBrowser;
} // namespace Ui

/**
 * @brief Widget for browsing actual datasets list.
 */
class DatasetsListBrowser : public QWidget
{
    Q_OBJECT
public:
    explicit DatasetsListBrowser(QWidget* parent = nullptr);

    ~DatasetsListBrowser() override;

    DatasetsListBrowser& operator=(const DatasetsListBrowser& other) = delete;
    DatasetsListBrowser(const DatasetsListBrowser& other) = delete;

    DatasetsListBrowser& operator=(DatasetsListBrowser&& other) = delete;
    DatasetsListBrowser(DatasetsListBrowser&& other) = delete;

    QString getSelectedDataset() const;

    void clearSelection();

    bool isDatasetsListEmpty();

private:
    Ui::DatasetsListBrowser* ui;

private slots:
    void searchTextChanged(const QString& arg1);

    void showContextMenu(const QPoint& pos);

    void on_datasetsList_itemSelectionChanged();

signals:
    void currentDatasetChanged(QString current);
};

#endif // DATASETSLISTBROWSER_H
