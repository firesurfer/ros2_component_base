/*
 * Copyright 2018 <Lennart Nachtigall> <firesurfer127@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef ENTITYWIDGET_H
#define ENTITYWIDGET_H

#include <QWidget>
#include "EntityBase.h"
#include "EntityModel.h"
#include <QVBoxLayout>
namespace Ui {
class EntityWidget;
}

namespace  ros2_components {


class EntityWidget : public QWidget
{
    Q_OBJECT

public:
    Q_INVOKABLE EntityWidget(EntityBase::SharedPtr _entity, QWidget *parent = 0);

    ~EntityWidget();

private:


protected:
    EntityModel* model;
    EntityBase::SharedPtr entity;
    Ui::EntityWidget *ui;
    QVBoxLayout* main_layout;
};

}
#endif // ENTITYWIDGET_H
