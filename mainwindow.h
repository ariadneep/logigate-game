/**
 * This class controls the UI for the EduGame application, as well
 * as UI-model connections.
 *
 * @authors Max Donaldson, Jason Khong,
 * Ariadne Petroulakis, Evan Tucker, and Ian Weller
 *
 * @version 7 April 2025
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    /**
     * @brief MainWindow - creates a main window object.
     * @param parent - the parent object
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * MainWindow destructor.
     */
    ~MainWindow();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
