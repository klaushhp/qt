#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QCloseEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QList>
#include <QUrl>
#include <QFileInfo>
#include <QObjectList>
#include <QMenu>
#include <QToolBar>
#include <QDebug>
#include <QPrintDialog>
#include <QPrinter>
#include <QKeyEvent>
#include <QApplication>
#include <QMimeData>

void MainWindow::showErrorMessage(QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("Error");
    msg.setText(message);
    msg.setIcon(QMessageBox::Critical);
    msg.setStandardButtons(QMessageBox::Ok);

    msg.exec();
}

int MainWindow::showQueryMessage(QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("Query");
    msg.setText(message);
    msg.setIcon(QMessageBox::Question);
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    return msg.exec();
}

QString MainWindow::showFileDialog(QFileDialog::AcceptMode mode, QString title)
{
    QString ret = "";
    QFileDialog fd(this);
    QStringList filters;
    QMap<QString, QString> map;
    const char* filterArray[][2] =
    {
        {"Text Files (*.txt)", ".txt"},
        {"All Files (*)",      "*"   },
        {NULL,                 NULL  }
    };

    for(int i=0; filterArray[i][0]!=NULL; i++)
    {
        filters.append(filterArray[i][0]);
        map.insert(filterArray[i][0], filterArray[i][1]);
    }

    fd.setWindowTitle(title);
    fd.setAcceptMode(mode);
    fd.setNameFilters(filters);

    if( mode == QFileDialog::AcceptOpen )
    {
        fd.setFileMode(QFileDialog::ExistingFile);
    }

    if( fd.exec() == QFileDialog::Accepted )
    {
        ret = fd.selectedFiles()[0];

        if( mode == QFileDialog::AcceptSave )
        {
            QString postfix = map[fd.selectedNameFilter()];

            if( (postfix != "*") && !ret.endsWith(postfix) )
            {
                ret = ret + postfix;
            }
        }
    }

    return ret;
}

void MainWindow::preEditorChange()
{
    if( m_isTextChanged )
    {
        int r = showQueryMessage("Do you want to save the changes to file?");

        switch(r)
        {
        case QMessageBox::Yes:
            saveCurrentData(m_filePath);
            break;
        case QMessageBox::No:
            m_isTextChanged = false;
            break;
        case QMessageBox::Cancel:
            break;
        }
    }
}

void MainWindow::onFileNew()
{
    preEditorChange();

    if( !m_isTextChanged )
    {
        mainEditor.clear();

        setWindowTitle("NotePad - [ New ]");

        m_filePath = "";

        m_isTextChanged = false;
    }
}

void MainWindow::openFileToEditor(QString path)
{
    if( path != "" )
    {
        QFile file(path);

        if( file.open(QIODevice::ReadOnly | QIODevice::Text) )
        {
            mainEditor.setPlainText(QString(file.readAll()));

            file.close();

            m_filePath = path;

            m_isTextChanged = false;

            setWindowTitle("NotePad - [ " + m_filePath + " ]");
        }
        else
        {
            showErrorMessage(QString("Open file error! \n\n") + "\"" + path + "\"");
        }
    }
}

void MainWindow::onFileOpen()
{
    preEditorChange();

    if( !m_isTextChanged )
    {
        QString path = showFileDialog(QFileDialog::AcceptOpen, "Open");

        openFileToEditor(path);
    }
}

QString MainWindow::saveCurrentData(QString path)
{
    QString ret = path;

    if( ret == "" )
    {
        ret = showFileDialog(QFileDialog::AcceptSave, "Save");
    }

    if( ret != "" )
    {
        QFile file(ret);

        if( file.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream out(&file);

            out << mainEditor.toPlainText();

            file.close();

            setWindowTitle("NotePad - [ " + ret + " ]");

            m_isTextChanged = false;
        }
        else
        {
            showErrorMessage(QString("Save file error! \n\n") + "\"" + ret + "\"");

            ret = "";
        }
    }

    return ret;
}

void MainWindow::onFileSave()
{
    QString path = saveCurrentData(m_filePath);

    if( path != "" )
    {
        m_filePath = path;
    }
}

void MainWindow::onFileSaveAs()
{
    QString path = saveCurrentData();

    if( path != "" )
    {
        m_filePath = path;
    }
}

void MainWindow::onTextChanged()
{
    if( !m_isTextChanged )
    {
        setWindowTitle("*" + windowTitle());
    }

    m_isTextChanged = true;
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    preEditorChange();

    if( !m_isTextChanged )
    {
        QMainWindow::closeEvent(e);
    }
    else
    {
        e->ignore();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent* e)
{
    if( e->mimeData()->hasUrls() )
    {
        e->acceptProposedAction();
    }
    else
    {
        e->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent* e)
{
    if( e->mimeData()->hasUrls() )
    {
        QList<QUrl> list = e->mimeData()->urls();
        QString path = list[0].toLocalFile();
        QFileInfo fi(path);

        if( fi.isFile() )
        {
            preEditorChange();

            if( !m_isTextChanged )
            {
                openFileToEditor(path);
            }
        }
        else
        {
            showErrorMessage("Cannot open a folder!");
        }
    }
    else
    {
        e->ignore();
    }
}

void MainWindow::onCopyAvailable(bool available)
{
    findMenuBarAction("Copy")->setEnabled(available);
    findMenuBarAction("Cut")->setEnabled(available);
    findToolBarAction("Copy")->setEnabled(available);
    findToolBarAction("Cut")->setEnabled(available);
}

void MainWindow::onRedoAvailable(bool available)
{
    findMenuBarAction("Redo")->setEnabled(available);
    findToolBarAction("Redo")->setEnabled(available);
}

void MainWindow::onUndoAvailable(bool available)
{
    findMenuBarAction("Undo")->setEnabled(available);
    findToolBarAction("Undo")->setEnabled(available);
}

QAction* MainWindow::findMenuBarAction(QString text)
{
    QAction* ret = NULL;
    const QObjectList& list = menuBar()->children();

    for(int i=0; i<list.count(); i++)
    {
       QMenu* menu = dynamic_cast<QMenu*>(list[i]);

       if( menu != nullptr )
       {
           QList<QAction*> actions = menu->actions();

           for(int j=0; j<actions.count(); j++)
           {
               if( actions[j]->text().startsWith(text) )
               {
                   ret = actions[j];
                   break;
               }
           }
       }
    }

    return ret;
}

QAction* MainWindow::findToolBarAction(QString text)
{
    QAction* ret = NULL;
    const QObjectList& list = children();

    for(int i=0; i<list.count(); i++)
    {
        QToolBar* toolBar = dynamic_cast<QToolBar*>(list[i]);

        if( toolBar != nullptr )
        {
            QList<QAction*> actions = toolBar->actions();

            for(int j=0; j<actions.count(); j++)
            {
                if( actions[j]->toolTip().startsWith(text) )
                {
                    ret = actions[j];
                    break;
                }
            }
        }
    }

    return ret;
}

void MainWindow::onFilePrint()
{
    QPrinter printer;
    QPrintDialog dlg(&printer, this);

    dlg.setWindowTitle("Print");

    if( dlg.exec() == QPrintDialog::Accepted )
    {
        mainEditor.document()->print(static_cast<QPagedPaintDevice *>(&printer));
    }
}

void MainWindow::onCursorPositionChanged()
{
    int pos = mainEditor.textCursor().position();
    QString text = mainEditor.toPlainText();
    int col = 0;
    int ln = 0;
    int flag = -1;

    for(int i=0; i<pos; i++)
    {
        if( text[i] == '\n' )
        {
            ln++;
            flag = i;
        }
    }

    flag++;

    col = pos - flag;

    statusLbl.setText("Ln: " + QString::number(ln + 1) + "    Col: " + QString::number(col + 1));
}

void MainWindow::onEditDelete()
{
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier);
    QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Delete, Qt::NoModifier);

    QApplication::sendEvent(&mainEditor, &keyPress);
    QApplication::sendEvent(&mainEditor, &keyRelease);
}

void MainWindow::onEditFind()
{
    m_pFindDlg->show();
}

void MainWindow::onEditReplace()
{
    m_pReplaceDlg->show();
}

void MainWindow::onFileExit()
{
    close();
}


