#pragma once
#include "Cmd/ClearScreenCmd.h"
#include "Cmd/RenderRectCmd.h"
#include "Cmd/SetBrushColorCmd.h"
#include "Cmd/SetRectCmd.h"

#include <libhelpers/Containers/PolyVector.h>

typedef containers::PolyVector
<
    ClearScreenCmd, RenderRectCmd,
    SetBrushColorCmd, SetRectCmd
>
RenderCmdList;