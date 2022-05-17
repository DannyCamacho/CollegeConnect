#ifndef ADMINMENU_H
#define ADMINMENU_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include "database.h"

namespace Ui { class AdminMenu; }

/*! \class AdminMenu
 *  \brief Handles maintenance of the database
 *  This class handles the ability to add, remove, modify and import files to the database.
 */
class AdminMenu : public QMainWindow {
    Q_OBJECT

public:

    /*! AdminMenu Constructor
     *
     *  Constructor used to initialized necessary variables and update the window.
     *  @param QWidget parent
     */
    explicit AdminMenu(QWidget *parent = nullptr);

    /*! AdminMenu Destructor
     *
     *  Destructor used to delete heap allocated memory.
     */
    ~AdminMenu();

private slots:

    /*! @fn void schoolTableViewUpdate()
     *
     *  void-returning method used to update the schoolModel based off the existing colleges.
     */
    void schoolTableViewUpdate();

    /*! @fn void souvenirTableViewUpdate()
     *
     *  void-returning method used to update the souvenir table based off the selected college.
     */
    void souvenirTableViewUpdate();

    /*! @fn void on_university_tableView_clicked(const QModelIndex &index)
     *
     *  void-returning method used to populate souvenir table when a college is selected, resets the souvenir lineEdits.
     *  @param const QModelIndex &index
     */
    void on_university_tableView_clicked(const QModelIndex &index);

    /*! @fn void on_add_button_clicked()
     *
     *  void-returning method used to add a new souvenir to the selected college. A warning popup is displayed if the input is not valid.
     */
    void on_add_button_clicked();

    /*! @fn void on_remove_button_clicked()
     *
     *  void-returning method used to remove an existing souvenir from the selected college. A warning popup is displayed if the input is not valid.
     */
    void on_remove_button_clicked();

    /*! @fn void on_save_changes_button_clicked()
     *
     *  void-returning method used to edit an existing souvenir from the selected college. A warning popup is displayed if the input is not valid.
     */
    void on_save_changes_button_clicked();

    /*! @fn void on_items_tableView_clicked(const QModelIndex &index)
     *
     *  void-returning method used to populate the souvenir lineEdits when a souvenir is selected.
     *  @param const QModelIndex &index
     */
    void on_items_tableView_clicked(const QModelIndex &index);

    /*! @fn on_actionLogout_triggered()
     *
     *  void-returning method used to move to the MainWindow.
     *  @see mainwindow.h
     */
    void on_actionLogout_triggered();

    /*! @fn void on_actionImport_University_triggered()
     *
     *  void-returning method used to import new universities using a selected file. A file selection window popup is displayed and the selected path is passed to the Database's importCollege method.
     */
    void on_actionImport_University_triggered();

    /*! @fn void on_actionImport_Souveners_triggered()
     *
     *  void-returning method used to import new souvenirs using a selected file. A file selection window popup is displayed and the selected path is passed to the Database's importSouvenir method.
     */
    void on_actionImport_Souveners_triggered();

signals:

    /*! @fn void adminLogout()
     *
     *  Signal used to notify MainWindow of adminstrative logout, returns user to MainWindow.
     *  @see mainwindow.h
     */
    void adminLogout();

private:

    /*! @var Ui::AdminMenu *ui
     *
     *  AdminMenu used to display and manage AdminMenu interactions.
     */
    Ui::AdminMenu *ui;

    /*! @var QSqlQueryModel* schoolModel
     *
     *  QSqlQueryModel used to display the list of colleges.
     */
    QSqlQueryModel* schoolModel;

    /*! @var QSqlQueryModel* souvenirModel
     *
     *  QSqlQueryModel used to display the souvenirs for the selected college.
     */
    QSqlQueryModel* souvenirModel;

    /*! @var Database database
     *
     *  Database instance used to modify the database by calling the import methods.
     */
    Database database;

    /*! @var std::string fileName
     *
     *  String used to hold the path for the import file.
     *  @see database.h
     */
    std::string fileName;

    /*! @var QString collegeName
     *
     *  QString used to hold the name of the selected college.
     */
    QString collegeName;

    /*! @var QString item
     *
     *  QString used to hold the name of the selected souvenir.
     */
    QString item;
};

#endif // ADMINMENU_H
