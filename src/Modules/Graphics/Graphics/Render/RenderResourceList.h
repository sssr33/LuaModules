#pragma once
#include "Resource/ColorBrushResource.h"
#include "Resource/RectResource.h"

#include <libhelpers/Containers/PolyVector.h>

typedef containers::PolyVector
<
    ColorBrushResource, RectResource
>
RenderResourceList;