#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <mutex>

#include <QObject>
#include <QColor>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>
#include <vtkTransformPolyDataFilter.h>

class Model : public QObject
{
	Q_OBJECT

public:
	Model();
	const vtkSmartPointer<vtkActor>& getModelActor() const;

	double getPositionX();
	double getPositionY();

	virtual void translateToPosition(const double x, const double y);

	void setSelected(const bool selected);
	static void setSelectedModelColor(const QColor &selectedModelColor);

	const double getMouseDeltaX() const;
	const double getMouseDeltaY() const;
	void setMouseDeltaXY(const double deltaX, const double deltaY);

	virtual void updateModelColor() = 0;

signals:
	void positionXChanged(const double positionX);
	void positionYChanged(const double positionY);

protected:
	void setPositionX(const double positionX);
	void setPositionY(const double positionY);

	virtual void setColor(const QColor &color) = 0;

	static QColor m_defaultModelColor;
	static QColor m_selectedModelColor;	

	vtkSmartPointer<vtkActor> m_modelActor;

	std::mutex m_propertiesMutex;

	double m_positionX{ 0.0 };
	double m_positionY{ 0.0 };
	double m_positionZ{ 0.0 };

	bool m_selected = false;

	double m_mouseDeltaX = 0.0;
	double m_mouseDeltaY = 0.0;
};

class PolyDataModel : public Model
{
	Q_OBJECT

public:
	PolyDataModel(vtkSmartPointer<vtkPolyData> modelData);
	virtual void translateToPosition(const double x, const double y) override;
	virtual void updateModelColor() override;

private:
	virtual void setColor(const QColor &color) override;

	vtkSmartPointer<vtkPolyData> m_modelData;
	vtkSmartPointer<vtkPolyDataMapper> m_modelMapper;
	vtkSmartPointer<vtkTransformPolyDataFilter> m_modelFilterTranslate;
};

#include "vtkDataSetMapper.h"
#include "vtkImageData.h"

class ImageModel : public Model
{
	Q_OBJECT

public:
	ImageModel(vtkSmartPointer<vtkImageData> modelData);
	virtual void translateToPosition(const double x, const double y) override;
	virtual void updateModelColor() override;

private:
	virtual void setColor(const QColor &color) override;

	vtkSmartPointer<vtkImageData> m_modelData;
	vtkSmartPointer<vtkDataSetMapper> m_modelMapper;	
};

#endif // MODEL_H
