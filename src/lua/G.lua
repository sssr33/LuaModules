
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
print('starting window create')
local window = graphics.RenderWindow.New('My Window')
print('window has been created')

local renderer = window.Renderer
print('got renderer')
local renderer2 = window.Renderer
print('got renderer2')
print('renderer == renderer2 : ' .. tostring(renderer == renderer2))

--sleep(3)
--
--window.Renderer = nil
--
--sleep(3)
--
--window.Renderer = renderer

--local renderer2 = window.Renderer23

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