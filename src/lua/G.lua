
local graphics = require 'Graphics'

function sleep(s)
  local ntime = os.time() + s
  repeat until os.time() > ntime
end

function printTable(tab, indent)
    indent = indent or ''

    for k,v in pairs(tab) do
        print(indent .. tostring(k) .. ' = ' .. tostring(v))

        if type(v) == 'table' then
            printTable(v, indent .. '  ')

            local mt = getmetatable(v)

            print(indent .. '  ' .. tostring(mt))

            if mt ~= nil then
                printTable(v, indent .. '  ')
            end
        end
    end
end

printTable(graphics)

local TestScene1 = function()
    local window = graphics.RenderWindow.New('My Window')
    local resourceSlots = graphics.RenderResourceSlots.New()

    resourceSlots.ColorBrush = 1
    resourceSlots.Rect = 2

    local r = 0.0
    local x = 0.0
    local frame = 0

    while frame < 400 do
        local renderList = graphics.RenderCmdList.New()

        renderList:ClearScreen({r, 1, 0})

        renderList:SetBrushColor({ 1, 0, 0 }, 0)
        renderList:SetRect(x + 10, 10, x + 100, 100, 0)
        renderList:SetRect(100, 100, 200, 200, 1)

        renderList:RenderRect(0, 0, true)
        renderList:RenderRect(1, 0, false)

        window:Render(renderList, resourceSlots)

        r = r + 0.01
        x = x + 0.1
        frame = frame + 1

        if r > 1.0 then
            r = 0.0
        end

        if x > 10.0 then
            x = 0.0
        end
    end
end

local TestScene2 = function()
    local window = graphics.RenderWindow.New('My Window')
    local resourceSlots = graphics.RenderResourceSlots.New()

    resourceSlots.ColorBrush = 1
    resourceSlots.Rect = 1

    local frame = 0

    while frame < 400 do
        local renderList = graphics.RenderCmdList.New()

        renderList:ClearScreen(0.1, 0.1, 0.1)

        for i = 1, 10 do
            local x1 = math.random(1000)
            local x2 = math.random(1000)
            local y1 = math.random(1000)
            local y2 = math.random(1000)

            local left = math.min(x1, x2)
            local top = math.min(y1, y2)
            local right = math.max(x1, x2)
            local bottom = math.max(y1, y2)

            renderList:SetRect(left, top, right, bottom, 0)

            local r = 1.0
            local g = 0.0
            local b = i / 10.0
            local a = 0.1 + 0.9 * (i / 10.0)

            renderList:SetBrushColor(r, g, b, a, 0)

            renderList:RenderRect(0, 0, true)
        end

        window:Render(renderList, resourceSlots)

        frame = frame + 1
    end
end

--TestScene1()
TestScene2()
--collectgarbage('collect')

print('program end')

io.read(1)