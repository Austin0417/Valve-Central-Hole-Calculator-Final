/****************************************************************************
** Meta object code from reading C++ file 'MeasureWidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../MeasureWidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MeasureWidget.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSMeasureWidgetENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMeasureWidgetENDCLASS = QtMocHelpers::stringData(
    "MeasureWidget",
    "UpdatePreviewMat",
    "",
    "OnPreviewMatResizeComplete",
    "Mat",
    "resized",
    "onAreaCalculationComplete",
    "valve_area"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMeasureWidgetENDCLASS_t {
    uint offsetsAndSizes[16];
    char stringdata0[14];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[27];
    char stringdata4[4];
    char stringdata5[8];
    char stringdata6[26];
    char stringdata7[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMeasureWidgetENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMeasureWidgetENDCLASS_t qt_meta_stringdata_CLASSMeasureWidgetENDCLASS = {
    {
        QT_MOC_LITERAL(0, 13),  // "MeasureWidget"
        QT_MOC_LITERAL(14, 16),  // "UpdatePreviewMat"
        QT_MOC_LITERAL(31, 0),  // ""
        QT_MOC_LITERAL(32, 26),  // "OnPreviewMatResizeComplete"
        QT_MOC_LITERAL(59, 3),  // "Mat"
        QT_MOC_LITERAL(63, 7),  // "resized"
        QT_MOC_LITERAL(71, 25),  // "onAreaCalculationComplete"
        QT_MOC_LITERAL(97, 10)   // "valve_area"
    },
    "MeasureWidget",
    "UpdatePreviewMat",
    "",
    "OnPreviewMatResizeComplete",
    "Mat",
    "resized",
    "onAreaCalculationComplete",
    "valve_area"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMeasureWidgetENDCLASS[] = {

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
       1,    0,   32,    2, 0x06,    1 /* Public */,
       3,    1,   33,    2, 0x06,    2 /* Public */,
       6,    1,   36,    2, 0x06,    4 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, QMetaType::Double,    7,

       0        // eod
};

Q_CONSTINIT const QMetaObject MeasureWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSMeasureWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMeasureWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMeasureWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MeasureWidget, std::true_type>,
        // method 'UpdatePreviewMat'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'OnPreviewMatResizeComplete'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const Mat &, std::false_type>,
        // method 'onAreaCalculationComplete'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>
    >,
    nullptr
} };

void MeasureWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MeasureWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->UpdatePreviewMat(); break;
        case 1: _t->OnPreviewMatResizeComplete((*reinterpret_cast< std::add_pointer_t<Mat>>(_a[1]))); break;
        case 2: _t->onAreaCalculationComplete((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MeasureWidget::*)();
            if (_t _q_method = &MeasureWidget::UpdatePreviewMat; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MeasureWidget::*)(const Mat & );
            if (_t _q_method = &MeasureWidget::OnPreviewMatResizeComplete; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MeasureWidget::*)(double );
            if (_t _q_method = &MeasureWidget::onAreaCalculationComplete; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *MeasureWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MeasureWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMeasureWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MeasureWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void MeasureWidget::UpdatePreviewMat()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MeasureWidget::OnPreviewMatResizeComplete(const Mat & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MeasureWidget::onAreaCalculationComplete(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
