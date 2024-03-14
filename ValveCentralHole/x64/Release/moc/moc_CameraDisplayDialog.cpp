/****************************************************************************
** Meta object code from reading C++ file 'CameraDisplayDialog.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../CameraDisplayDialog.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CameraDisplayDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSCameraDisplayDialogENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSCameraDisplayDialogENDCLASS = QtMocHelpers::stringData(
    "CameraDisplayDialog",
    "SendImageMatToLabel",
    "",
    "pixmap",
    "OnCameraOpenFailed",
    "OnImageCaptured",
    "Mat",
    "captured_mat"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSCameraDisplayDialogENDCLASS_t {
    uint offsetsAndSizes[16];
    char stringdata0[20];
    char stringdata1[20];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[19];
    char stringdata5[16];
    char stringdata6[4];
    char stringdata7[13];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSCameraDisplayDialogENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSCameraDisplayDialogENDCLASS_t qt_meta_stringdata_CLASSCameraDisplayDialogENDCLASS = {
    {
        QT_MOC_LITERAL(0, 19),  // "CameraDisplayDialog"
        QT_MOC_LITERAL(20, 19),  // "SendImageMatToLabel"
        QT_MOC_LITERAL(40, 0),  // ""
        QT_MOC_LITERAL(41, 6),  // "pixmap"
        QT_MOC_LITERAL(48, 18),  // "OnCameraOpenFailed"
        QT_MOC_LITERAL(67, 15),  // "OnImageCaptured"
        QT_MOC_LITERAL(83, 3),  // "Mat"
        QT_MOC_LITERAL(87, 12)   // "captured_mat"
    },
    "CameraDisplayDialog",
    "SendImageMatToLabel",
    "",
    "pixmap",
    "OnCameraOpenFailed",
    "OnImageCaptured",
    "Mat",
    "captured_mat"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSCameraDisplayDialogENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   32,    2, 0x06,    1 /* Public */,
       4,    0,   35,    2, 0x06,    3 /* Public */,
       5,    1,   36,    2, 0x06,    4 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPixmap,    3,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,

       0        // eod
};

Q_CONSTINIT const QMetaObject CameraDisplayDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSCameraDisplayDialogENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSCameraDisplayDialogENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSCameraDisplayDialogENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CameraDisplayDialog, std::true_type>,
        // method 'SendImageMatToLabel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QPixmap, std::false_type>,
        // method 'OnCameraOpenFailed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'OnImageCaptured'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Mat, std::false_type>
    >,
    nullptr
} };

void CameraDisplayDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CameraDisplayDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->SendImageMatToLabel((*reinterpret_cast< std::add_pointer_t<QPixmap>>(_a[1]))); break;
        case 1: _t->OnCameraOpenFailed(); break;
        case 2: _t->OnImageCaptured((*reinterpret_cast< std::add_pointer_t<Mat>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CameraDisplayDialog::*)(QPixmap );
            if (_t _q_method = &CameraDisplayDialog::SendImageMatToLabel; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CameraDisplayDialog::*)();
            if (_t _q_method = &CameraDisplayDialog::OnCameraOpenFailed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CameraDisplayDialog::*)(Mat );
            if (_t _q_method = &CameraDisplayDialog::OnImageCaptured; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *CameraDisplayDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CameraDisplayDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSCameraDisplayDialogENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int CameraDisplayDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CameraDisplayDialog::SendImageMatToLabel(QPixmap _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CameraDisplayDialog::OnCameraOpenFailed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CameraDisplayDialog::OnImageCaptured(Mat _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
