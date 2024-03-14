/****************************************************************************
** Meta object code from reading C++ file 'CalibrateWidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../CalibrateWidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CalibrateWidget.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSCalibrateWidgetENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSCalibrateWidgetENDCLASS = QtMocHelpers::stringData(
    "CalibrateWidget",
    "UpdatePreviewMat",
    "",
    "OnCalibrationComplete",
    "calibration_factor",
    "ShouldClearHelperGaugeLines",
    "MirrorDrawnLinesToPreview"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSCalibrateWidgetENDCLASS_t {
    uint offsetsAndSizes[14];
    char stringdata0[16];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[22];
    char stringdata4[19];
    char stringdata5[28];
    char stringdata6[26];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSCalibrateWidgetENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSCalibrateWidgetENDCLASS_t qt_meta_stringdata_CLASSCalibrateWidgetENDCLASS = {
    {
        QT_MOC_LITERAL(0, 15),  // "CalibrateWidget"
        QT_MOC_LITERAL(16, 16),  // "UpdatePreviewMat"
        QT_MOC_LITERAL(33, 0),  // ""
        QT_MOC_LITERAL(34, 21),  // "OnCalibrationComplete"
        QT_MOC_LITERAL(56, 18),  // "calibration_factor"
        QT_MOC_LITERAL(75, 27),  // "ShouldClearHelperGaugeLines"
        QT_MOC_LITERAL(103, 25)   // "MirrorDrawnLinesToPreview"
    },
    "CalibrateWidget",
    "UpdatePreviewMat",
    "",
    "OnCalibrationComplete",
    "calibration_factor",
    "ShouldClearHelperGaugeLines",
    "MirrorDrawnLinesToPreview"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSCalibrateWidgetENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   38,    2, 0x06,    1 /* Public */,
       3,    1,   39,    2, 0x06,    2 /* Public */,
       5,    0,   42,    2, 0x06,    4 /* Public */,
       6,    0,   43,    2, 0x06,    5 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    4,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject CalibrateWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSCalibrateWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSCalibrateWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSCalibrateWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CalibrateWidget, std::true_type>,
        // method 'UpdatePreviewMat'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'OnCalibrationComplete'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'ShouldClearHelperGaugeLines'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'MirrorDrawnLinesToPreview'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void CalibrateWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CalibrateWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->UpdatePreviewMat(); break;
        case 1: _t->OnCalibrationComplete((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 2: _t->ShouldClearHelperGaugeLines(); break;
        case 3: _t->MirrorDrawnLinesToPreview(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CalibrateWidget::*)();
            if (_t _q_method = &CalibrateWidget::UpdatePreviewMat; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CalibrateWidget::*)(double );
            if (_t _q_method = &CalibrateWidget::OnCalibrationComplete; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CalibrateWidget::*)();
            if (_t _q_method = &CalibrateWidget::ShouldClearHelperGaugeLines; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CalibrateWidget::*)();
            if (_t _q_method = &CalibrateWidget::MirrorDrawnLinesToPreview; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *CalibrateWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CalibrateWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSCalibrateWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CalibrateWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CalibrateWidget::UpdatePreviewMat()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CalibrateWidget::OnCalibrationComplete(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CalibrateWidget::ShouldClearHelperGaugeLines()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void CalibrateWidget::MirrorDrawnLinesToPreview()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
