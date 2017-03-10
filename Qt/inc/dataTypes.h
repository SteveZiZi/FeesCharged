#pragma once



namespace CCT
{
    enum CARD_TYPE {
        NO_CARD,            // not insert card
        USER_CARD,          // user card inserted
        MAINTAINER_CARD     // maintainer card inserted
    };

    enum CARD_STATUS {
        NONE_CARD,
        INSERT_CARD,        // user card or maintainer card inserted
        REMOVE_CARD         // user card or maintainer card removed
    };
};