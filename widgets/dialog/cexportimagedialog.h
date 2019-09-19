#ifndef CEXPORTIMAGEDIALOG_H
#define CEXPORTIMAGEDIALOG_H


#include <DWidget>
#include <DDialog>
#include <DLineEdit>
#include <DComboBox>
#include <DLabel>
#include <DSlider>

DWIDGET_USE_NAMESPACE

class CExportImageDialog : public DDialog
{
    Q_OBJECT

    enum ESaveFormat {
        JPG,
        PNG,
        BMP,
        TIF,
        PDF
    };

    enum ESavePath {
        Pictures,
        Documents,
        Downloads,
        Desktop,
        Videos,
        Music,
        UsrSelect,
        Other
    };

public:
    explicit CExportImageDialog(DWidget *parent = nullptr);
    ~CExportImageDialog();

    void showMe(const QPixmap &pixmap);

private slots:
    void slotOnSavePathChange(int index);
    void slotOnFormatChange(int index);
    void slotOnDialogButtonClick(int index, const QString &text);
    void slotOnQuestionDialogButtonClick(int index, const QString &text);
    void slotOnQualityChanged(int value);

private:
    DLineEdit *m_fileNameEdit;
    DComboBox *m_savePathCombox;
    DComboBox *m_formatCombox;
    DSlider *m_qualitySlider;
    DLabel *m_qualityLabel;

//    QString m_fileName;
    QString m_savePath;
    QString m_saveFormat;
    int m_quality;

    QPixmap m_saveImage;

    DDialog *m_questionDialog;


private:
    void initUI();
    void initConnection();

    void showDirChoseDialog();
    void showQuestionDialog(const QString &path);
    void doSave();
};

#endif // CEXPORTIMAGEDIALOG_H