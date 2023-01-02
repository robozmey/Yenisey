//
// Created by vladimir on 28.12.22.
//

#ifndef YENISEY_Component_H
#define YENISEY_Component_H

namespace yny {

    enum ComponentType {
        Transform,
        Rigibody
    };

    class Component {
        ComponentType type;
    };

} // yny

#endif //YENISEY_Component_H
