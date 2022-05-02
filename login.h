#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QMessageBox>

namespace Ui { class Login; }

/*! \class Login
 *  \brief Handles login to admin
 *  This class handles the ability to login as an admin to access the debug menu
 *  @see debugmenu.h
 */
class Login : public QDialog {
    Q_OBJECT

public:

    /*! Constructor
     *
     *  Constructor used to connect to the existing MainWindow UI.
     *  @param QWidget parent
     */
    explicit Login(QWidget *parent = nullptr);

    /*! Destructor
     *
     *  Destructor used to delete Login UI.
     */
    ~Login();

signals:
    /*! @fn void sendMessage(const QString &msg)
     *
     *  @param const QString &msg
     */
    void sendMessage(const QString &msg);

private slots:

    /*! @fn void on_pushButtonLogin_clicked()
     *
     *  Login Button: Checks that both username and password match the predetermined QStrings, if both match then the MainWindow UI is notified and the Login UI is deleted. If the input strings do not match, the a warning popup is shown to the user.
     */
    void on_pushButtonLogin_clicked();

    /*! @fn void on_pushButtonCancel_clicked()
     *
     *   Cancel Button: hides and deletes the Login UI.
     */
    void on_pushButtonCancel_clicked();

    /*! @fn void on_lineEditPassword_returnPressed()
     *
     *  Return key pressed on password line edit: calls the pushButton clicked method.
     */
    void on_lineEditPassword_returnPressed();

private:

    /*! @var Ui::Login *ui
     *  Login's ui
     */
    Ui::Login *ui;
};

#endif // LOGIN_H
