local warp = 0 -- You can save shader IDs here and modify the properties of it using the ID

function _print(msg)
	print("[LUA] " .. msg) -- prints to console with [LUA] tag (easier tod ebug)
end

function modulo(a, b)
	return a % b
end

	-- yes you can have multiple shaders of same type too

	-- Create a global shader
	-- warp = createShader("warp.frag","shader.vert", 0)

	-- Create a background shader
	-- background = createBackgroundShader("background/circles.frag","shader.vert", 0)

	-- Create an overlay shader
	-- dvd = createOverlayShader("dvd.frag","shader.vert", 0)

	-- Enable/disable a shader
	-- manageShader(warp, 0) -- 0 = disable, 1 = enable (lua did not allow booleans)
	-- the last number on every creation function is also this same property

	-- Load a modifiable shader property (needed so you can modify them)
	-- queueShaderProperty(shader, "property")

	-- Modify a shader property
	-- modifyShaderProperty(shader, "property", value)

	-- Set the mask for shader
	-- setShaderMask(shader, mask)
	-- (mask is image name, needs to be png, in images folder)

	-- Modifier with easing
	-- meMod(step, strength, "linear", time, "modName");

	-- Modifier without easing
	-- setMod(step, strength, "modName");

	--[[
		All the modifiers you can use:
		- beat
		- flip
		- confusion
		- dizzy
		- drunk
		- tipsy
		- invert
		- movex
		- movey
		- reverse
		- split
		- cross
		- alternate

		(idk what the a means its just taro template modifiers lol)
		- adrunk
		- atipsy
		- amovex
		- amovey

		you can also call any modifier name and it calls lua function (doesnt return anything to modchart tho so ur gonna have to edit the data manually)
		to edit data, you can use setModifierOffset(x,y)

		you can also edit specific receptor position by using setReceptorPosition(id,x,y)
		same with playfield, using setPlayfieldPosition(id, x,y)
		havent added multiple playfield support yet tho

		you can also do flashes using flash(r,g,b,duration)
		setWindowSize also exists but it kills your memory so please dont
		setWindowPosition works tho (x,y)

		TO PORT SHADERS I RECOMMEND USING https://github.com/Gargaj/Bonzomatic ITS SO USEFUL FOR TESTING IT
		IF IT WORKS THERE IT WORKS HERE
	]]

local background

local properties = {
	intensity = 0.5,
	speed = 0.5,
}

local bt;

local orders = {
	{
		-25,
		0,
		25,
		50,
	},
	{
		25,
		0,
		-25,
		-50,
	},
	{
		50,50,
		-50,50,
	},
	{
		-50,-50,
		50,50,
	},
	{
		-50,50,
		50,-50
	}
}

local orderBeats = {68.25}

function init()
	--background = createBackgroundShader("background/fuji.frag","shader.vert", 0)

	setMod(0,1000,'movex',0)
	meMod(3,1,e('OutCubic'),0,'movex',0)
	meMod(8,3,e('InOutSine'),-0.5,'flip',0)
	meMod(8,3,e('InOutSine'),0.25,'tipsy',0)
	meMod(11.5,1,e('InOutSine'),0,'tipsy',0)
	meMod(11.5,1,e('InOutSine'),0.5,'drunk',0)
	meMod(15,3,e("InOutSine"),0,'flip',0)

	local r1 = 0
	for _ = 16,19 do
		r1 = r1 + 0.25
		meMod(_,0.1,e("linear"),r1,"invert",0)
	end

	meMod(20,0.25,e("OutExpo"),0,"invert",0)
	meMod(20,0.25,e("OutExpo"),0,"flip",0)
	meMod(20.4,0.15,e("Linear"),0.35,'tipsy',0)
	meMod(22.1,0.45,e("ExpoOut"),0,'tipsy',0)

	meMod(20.4,0.15,"_",0.25,'drunk',0)
	meMod(20.4,0.15,'_',0.25,'dizzy',0)

	meMod(26,0.75,e'InCube',-0.25,'flip',0)
	meMod(27,0.75,e'InCube',0,'flip',0)

	meMod(34,2,e'InOutCube',1,'reverse',0)

	meMod(36,2,e'InOutSine',-0.15,'flip',0)
	meMod(36,2,'_',0,'dizzy',0)

	for _, oBT in pairs(orderBeats) do
		setMod(oBT,100,'movex',0)
		meMod(oBT+.5,2,e'OutExpo',0,'movex',0)

		for i = 1,4 do
			setMod(oBT,orders[math.random(1,#orders)[i]],'movey',0)
		end
	end
end


local winX=0
function update(elapsed, ticks, songPos)
	if bt >= 36 and bt <= 68 then
		local wave_offset_y = 100 * math.sin((ticks * properties.speed) * math.pi * 0.5) * properties.intensity; -- offset math, stolen from maelstrom
		setWindowPosition(0, wave_offset_y); -- sets window position to offset
	end

	if bt >= 1111 then
		if winX >= 1570 then winX = -1570 end

		local wave_offset_y = 100 * math.sin((ticks * properties.speed) * math.pi * 0.5) * properties.intensity; -- offset math, stolen from maelstrom

		winX = winX + 15
		setWindowPosition(winX, wave_offset_y); -- sets window position to offset
	end
end

function stepHit(step, stepCrochet)
	if step >= 265 and step <= 272 then
		local v
		if step % 2 == 0 then v = 1 else v = 0 end
		manageShader(background,v)
	end
	if step == 273 then
		flash(255,255,255,1)
		manageShader(background,0)
	end
	if step >= 273 then
		setMod(step/4,randomC,'confusion',0)
	end
	if step == 278 then
		setWindowPosition(0, 0);
	end

end

function beatHit(beat, crochet)
	bt = beat
	if bt == 36 then
		flash(255,255,255,1)
		manageShader(background,1)
	end
end

function e(n)
	return 'ease'..n
end