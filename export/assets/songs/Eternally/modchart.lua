local global_dt = 0

local properties = {
	intensity = 0.5,
	speed = 0.5,
}

local pixel_property = 0
local lane_underlay = 0

local curBeat = 0
local curStep = 0

local multX = 640 / 1280
local multY = 480 / 720

local flicker = 1
local text_id = 0

local shaders = {
	"warp.frag",
}

local backgroundShaders = {
	"background/circles.frag",
}

local shader_ids = {}
local bg_shader_ids = {}
local shader_properties = {}
local playfields = {}

local dir = io.popen("cd"):read()
package.path = dir .. "\\assets\\scripts\\?.lua"

function _print(msg)
	print("[LUA] " .. msg) -- prints to console with [LUA] tag
end

function modulo(a, b)
	return a % b
end

function setPos(x, y)
	setWindowPosition(x, y)
	setPlayfieldPosition(-x, -y)
end

function init()
	math.randomseed(os.time())

	setWindowSize(1280, 720)
	setPos(0, 0)

	for i = 1, #backgroundShaders do
		_print("Loading shader " .. backgroundShaders[i] .. ".frag")
		bg_shader_ids[i] = createBackgroundShader(backgroundShaders[i],"shader.vert",0)
		_print("Loaded shader " .. backgroundShaders[i] .. " with id " .. bg_shader_ids[i])
	end


	for i = 1, #shaders do
		_print("Loading shader " .. shaders[i] .. ".frag")
		shader_ids[i] = createShader(shaders[i],"shader.vert",0)
		_print("Loaded shader " .. shaders[i] .. " with id " .. shader_ids[i])

		if shaders[i] == "hdr" then
			shader_properties[i] = queueShaderProperty(shader_ids[i], "exposure")
		end
	end

	--lane_underlay = createRectUnderlay(430,0,420,720,0,0,0,192) -- creates rectangle for underlay

	--_print("Shader ID: " .. shader)
	--_print("Lane Underlay ID: " .. lane_underlay)

	-- create text
	--fps->create("FPS: 0\nMemory: 0mb", con(FONT_PATH, "vcr.ttf"), 16, 10, 10, {255, 255, 255, 0});
end

function perframe(t1,t2)
	return (curBeat >= t1 and curBeat <= t2)
end

function update(elapsed, ticks, songPos)
	curBeat = (songPos)*(128/60)
	flicker = -flicker
	global_dt = elapsed; -- sets global dt to elapsed time for other scripts

	--modifyShaderProperty(shader_ids[1], shader_properties[1], math.sin(ticks * math.pi * 0.5))

	if perframe(3.75, 31.5) then
		local curtime = modulo(curBeat * 4, 3) / 3
		local amount = 640 * (curBeat - 4) / 28
		local mult = modulo(math.floor(curBeat * 4 / 3), 2) * 2 - 1
		setPos(mult * amount * (curtime * (curtime - 2) + 1) * multX, 0)
	end

	if perframe(32, 64) then
		local curtime = modulo(curBeat * 4, 3) / 3

		local amount = 240 * (curBeat - 32) / 32
		if curBeat > 60 then
			amount = 240 * (1 - (curBeat - 60) / 4)
		end
		local x = math.sin(curBeat * math.pi * 0.5) * amount
		local y = math.cos(curBeat * math.pi * 0.5) * amount
		setPos(x * multX, y * multY)
	end

	if perframe(64, 71.5) or perframe(80, 87.5) then
		local off = 1
		if curBeat >= 80 then off = 2 end
		local curtime = modulo(curBeat * 4 - off, 3) / 3
		setPos(0, (-120 + 240 * curtime) * multY)
	end

	if perframe(71.5, 80) then
		local off = 0.405
		if curBeat >= 80 then off = 0.125 end
		local curtime = (curBeat - 71.5)/6.5
		if curBeat > 80 then curtime = (curBeat - 87.5)/6.5 end
		setPos(math.sin(((curBeat * 0.33) + off) * math.pi) * 60 * multX, (120 - 120 * curtime * curtime) * multY)
	end

	if perframe(88, 96) then
		local curtime = curBeat - math.floor(curBeat)
		local mult = modulo(math.floor(curBeat), 2) * 2 - 1
		setPos((curtime * (curtime - 2) + 1) * mult * 240 * multX, (curtime * (curtime - 2) + 1) * 100 * multY)
	end

	if perframe(97, 128) then
		local dir = 0.5 + 0.6 * (curBeat - 96) / 32

		if modulo(curBeat, 2) < 1 then
			setPos(0, math.sin(curBeat * math.pi) * flicker * 240 * dir)
		else
			setPos(math.sin(curBeat * math.pi) * flicker * 240 * dir * multX, 0)
		end
	end

	if perframe(128, 129) then
		setPos(0, 0)
	end

	if perframe(129, 160) then
		local dir = 0.5 + 0.75 * (curBeat - 128) / 32
		if curBeat > 156 then dir = 1.25 - 1.25 * (curBeat - 156) / 4 end

		local x = math.sin(curBeat * math.pi) * flicker * 240 * dir
		local y = math.cos(curBeat * math.pi) * flicker * 240 * dir

		setPos(x * multX, y * multY)
	end

	if perframe(160, 161) then
		setPos(0, 0)
	end

	if perframe(160, 191) then
		local cur = modulo(math.floor(curBeat), 4)
		local curtime = curBeat - math.floor(curBeat)
		local ease = -curtime * (curtime - 2)
		local x, y = 0, 0

		if cur == 0 then
			x = ease * 320 - 320
			y = ease * 240
		elseif cur == 1 then
			x = ease * 320
			y = ease * -240 + 240
		elseif cur == 2 then
			x = ease * -320 + 320
			y = ease * -240
		else
			x = ease * -320
			y = ease * 240 + -240
		end

		setPos(x * flicker * multX, y * flicker * multY)
	end

	if perframe(191, 192) then
		setPos(0, 0)
	end

	if perframe(193, 224) or perframe(225, 256) then

		local rot = modulo(curBeat * 180, 360)

		local mult = 1
		if curBeat > 252 then mult = 1 - (curBeat - 252) / 4 end

		local cur = modulo(math.floor(curBeat), 4)
		local curtime = curBeat - math.floor(curBeat)
		local ease = -curtime * (curtime - 2)
		local x, y = 0, 0
		local x2, y2 = 0, 0

		local w, h = 1280, 720
		if modulo(math.floor(curBeat), 2) == 0 and curBeat > 226 then
			w, h = 1408, 1056
		end

		if cur == 0 then
			x = -320
			y = ease * -480 + 240
		elseif cur == 1 then
			x = ease * 640 - 320
			y = -240
		elseif cur == 2 then
			x = 320
			y = ease * 480 - 240
		else
			x = ease * -640 + 320
			y = 240
		end

		x, y = x * mult, y * mult

		setPos(x * flicker * multX, y * flicker * multY)
		setWindowSize(w,h)
	end

	if perframe(224, 225) or perframe(256, 257) then
		setPos(0, 0)
		setWindowSize(1280,720)
	end

	if perframe(256, 284) then
		local mult = modulo(math.floor(curBeat), 2) * 2 - 1
		setPos(((curBeat - math.floor(curBeat)) * 120 * mult - 60 * mult) * multX, (math.abs(math.sin(math.pi * curBeat)) * -120 + 60) * multY)
	end

	if perframe(284, 284.75) then
		local curtime = (curBeat - 284)/0.75
		setPos((60 + 180 * (1 - math.pow(2, -10 * curtime))) * multX, 0)
	end
	if perframe(284.75, 285.5) then
		local curtime = (curBeat - 284.75)/0.75
		setPos((240 - 480 * (1 - math.pow(2, -10 * curtime))) * multX, 0)
	end
	if perframe(285.5, 286.25) then
		local curtime = (curBeat - 285.5)/0.75
		setPos((-240 + 480 * (1 - math.pow(2, -10 * curtime))) * multX, 0)
	end
	if perframe(286.25, 287.5) then
		local curtime = (curBeat - 286.25)/1.25
		setPos((240 - 240 * (1 - math.pow(2, -10 * curtime))) * multX, 0)
	end

	if perframe(287.5, 288.5) then
		local curtime = curBeat - 287.5
		setPos(0,0)
		setWindowSize(((1 - math.pow(2, -10 * curtime)) + 1) * 1280, ((-(1 - math.pow(2, -10 * curtime)) + 1) * 720) - 1)
	end

	if curBeat > 292 then
		setPos(0,0)
		setWindowSize(1280, 720)
	end
end

function stepHit(step,stepCrochet)
	curStep = step
end

local prev_id = nil

function beatHit(beat,crochet)
	--[[if beat == 4 then
		text_id = createText("test", "vcr.ttf", 16, 10, 10, 255, 255, 255)
		setTextOutline(text_id, 1, 0, 0, 0)
		centerText(text_id)
	end]]
	--curBeat = beat
	--[[if beat == 32 then
		properties.intensity = 0 -- sets intensity to 0
		properties.speed = 0 -- sets speed to 0
		manageShader(shader,0) -- disables shader
	end]]
end

