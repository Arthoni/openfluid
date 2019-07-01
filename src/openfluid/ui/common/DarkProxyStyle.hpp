#ifndef DARKPROXYSTYLE_H
#define DARKPROXYSTYLE_H


#include <QPalette>
#include <QProxyStyle>


class QPainterPath;

class DarkProxyStyle : public QProxyStyle
{
    Q_OBJECT
public:
    DarkProxyStyle();

    void polish(QPalette &palette) override;
    void polish(QWidget *widget) override;
    void unpolish(QWidget *widget) override;
    int pixelMetric(PixelMetric metric, const QStyleOption *option,
                    const QWidget *widget) const override;
    int styleHint(StyleHint hint, const QStyleOption *option,
                  const QWidget *widget, QStyleHintReturn *returnData) const override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const override;
    void drawControl(ControlElement control, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const override;

private:
    static void setTexture(QPalette &palette, QPalette::ColorRole role,
                           const QPixmap &pixmap);
    static QPainterPath roundRectPath(const QRect &rect);
};


#endif // DARKPROXYSTYLE_H