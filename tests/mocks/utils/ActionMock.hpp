/* Created by kblaszki */
#pragma once

#include <gmock/gmock.h>

struct ActionMock
{
    MOCK_METHOD(void, doAction, (), ());
};