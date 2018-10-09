
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

--do
--local renderer = graphics.GraphicsRenderer.New()
--local renderer2 = graphics.GraphicsRenderer.New()
--
--print('renderer2 table is ' .. tostring(renderer2))
--
--print('before local p = renderer.MyProp')
--print('renderer table is ' .. tostring(renderer))
--local p = renderer.MyProp
--print('after local p = renderer.MyProp')
--
--end

do
    local renderList = graphics.RenderCmdList.New()

    renderList:ClearScreen({1, 2, 3})

    renderList:RenderRect(1, 2, false)

    print('\nrenderList MT:')
    -- funny bug with console output here :), looks like recursion with __index table :)
    -- printTable(getmetatable(renderList))
    print('\n')

    local renderResourceSlots = graphics.RenderResourceSlots.New()

    local mt1 = getmetatable(renderResourceSlots)

    print('\nrenderResourceSlots MT:')
    printTable(mt1)
    print('\n')

    local rects = renderResourceSlots.Rect
    renderResourceSlots.ColorBrush = 12

    local rect2 = renderResourceSlots.Rect
    renderResourceSlots.Rect = 22

    --renderResourceSlots.asdasd = 'asd'
end

collectgarbage('collect')

do
    print('starting window create')
    local window = graphics.RenderWindow.New('My Window')
    print('window has been created')
    
    local renderer = window.Renderer
    print('got renderer')
    local renderer2 = window.Renderer
    print('got renderer2')
    print('renderer == renderer2 : ' .. tostring(renderer == renderer2))
    
    local renderer23 = window.UnknownKey
    print('UnknownKey is ' .. tostring(renderer23))
    
    sleep(2)
    
    window.Renderer = nil
    
    if window.Renderer == nil then
        print('window.Renderer == nil')
    else
        print('window.Renderer is not nil')
    end
    
    sleep(2)
    
    window.Renderer = renderer
    
    local renderer2 = window.Renderer23

end

--io.read(1)
--collectgarbage('collect')

for k,v in pairs(_G) do
    --print(k .. ' = ' .. tostring(v))
end

--mt1 = {}
--
--mt1.__index = mt1
--mt1.__newindex = function(t, k, v)
--    print('mt1.__newindex: ' .. tostring(k) .. ' = ' .. tostring(v))
--    print('mt1.new mt1==t ' .. tostring(t == mt1))
--    rawset(t, k, v)
--end
--
--function mt1:new(o)
--    o = o or {}
--    setmetatable(o, self)
--    --print('mt1.new mt1==self ' .. tostring(self == mt1))
--    return o
--end
--
--mt2 = mt1:new()
--
--function mt2:mt2Test()
--end
--
--o1 = mt1:new()
--
--o1.zzz = 23
--o1.zzz = 45
--
--o2 = mt2:new()
--
--o2.zzz = 23
--o2.zzz = 26

print('program end')

io.read(1)