local global_dt = 0

local properties = {
	intensity = 0.5,
	speed = 0.5,
}

local warp = 0
local infinite = 0
local background = 0

local ARROW_SIZE = 100
local audio_pos = 0

function _print(msg)
	print("[LUA] " .. msg) -- prints to console with [LUA] tag
end

function modulo(a, b)
	return a % b
end

function customModifier(col)
	--_print("customModifier called")
	setModifierOffset(0, 0, (cos((audio_pos * 1000) * 0.001 * (1.2) + col * (2.0) + 1 * (0.2)) * ARROW_SIZE * 0.4))
end

function init()
	--warp = createShader("warp.frag","shader.vert", 1)
	--background = createBackgroundShader("background/circles.frag","shader.vert", 0)
	--infinite = createShader("infinite.frag","shader.vert", 0)

	meMod(0, 2, "easeOutBounce", 0.5, "tipsy", 0);
	meMod(4, 2, "easeOutSine", 1, "reverse", 0);

	meMod(8, 2, "easeOutSine", 0, "reverse", 0);
	meMod(8, 2, "easeOutQuad", 1, "split", 0);

	meMod(12, 2, "easeOutQuad", 0, "split", 0);
	meMod(12, 2, "easeOutCubic", 1, "cross", 0);

	meMod(16, 2, "easeOutCubic", 0, "cross", 0);
	meMod(16, 2, "easeOutQuart", 1, "alternate", 0);

	meMod(20, 2, "easeOutQuart", 0, "alternate", 0);
	meMod(20, 2, "easeOutQuint", 1, "confusion", 0);

	meMod(24, 2, "easeOutExpo", 0, "confusion", 0);
	meMod(24, 2, "easeOutExpo", 2, "drunk", 0);

	meMod(28, 2, "easeOutElastic", 0, "drunk", 0);
	meMod(28, 2, "easeOutElastic", 1, "dizzy", 0);

	meMod(30, 2, "easeLinear", 0, "dizzy", 0);

	setMod(32, 0, "tipsy");
	setMod(32, 1, "beat");

	setMod(64, 0, "beat");
	setMod(64, 0.5, "alpha");

	setMod(72, 1, "alpha");
	setMod(72, 1, "beat");
	setMod(72, 0.5, "tipsy");

	setMod(128, 0, "beat");

	setMod(160, 0.25, "tipsy");

	setMod(193, 1, "beat");

	setMod(256, 0, "beat");
	setMod(256, 0, "tipsy");
	setMod(256, 0, "dizzy");
	setMod(256, 0, "drunk");
end

function perframe(t1,t2)
	return (beat >= t1 and beat <= t2)
end

function update(elapsed, ticks, songPos)
	audio_pos = songPos
	global_dt = elapsed; -- sets global dt to elapsed time for other scripts

	local wave_offset_x = 50 * math.sin((ticks * properties.speed) * math.pi) * properties.intensity; -- offset math, stolen from maelstrom
	local wave_offset_y = 50 * math.sin((ticks * properties.speed) * math.pi * 0.5) * properties.intensity; -- offset math, stolen from maelstrom
	local size_x = 640 * math.sin((ticks * properties.speed) * math.pi * 0.5) * properties.intensity;
	local size_y = 240 * math.sain((ticks * properties.speed) * math.pi * 0.5) * properties.intensity;

	setPlayfieldPosition(-wave_offset_x, -wave_offset_y);
	setWindowPosition(wave_offset_x, wave_offset_y); -- sets window position to offset

	setModifierOffset(0, 0, (cos((audio_pos * 1000) * 0.001 * (1.2) + col * (2.0) + 1 * (0.2)) * ARROW_SIZE * 0.4))
end

function stepHit(step,stepCrochet)
	curStep = step
end

function beatHit(beat,crochet)
	curBeat = beat
	if beat == 0 then

	elseif beat == 32 then
		properties.intensity = 0
		properties.speed = 0

		flash(255,255,255,1)
		--manageShader(background,1)
		--manageShader(warp,0)
	elseif beat == 64 then
		--modifyRectColor(black_rect,0,0,0,255)
		flash(255,255,255,1)
	elseif beat == 72 then
		properties.intensity = 0.25
		properties.speed = 0.25
		--modifyRectColor(black_rect,0,0,0,0)
		flash(255,255,255,1)
	elseif beat == 128 then
		properties.intensity = 0.5
		properties.speed = 0.5

		flash(255,255,255,1)
		--manageShader(background,0)
		--manageShader(warp,1)
	elseif beat == 160 then
		properties.intensity = 0.25
		properties.speed = 0.25
		--modifyRectColor(black_rect,0,0,0,255)
		flash(255,255,255,1)
	elseif beat == 193 then

		modifyRectColor(black_rect,0,0,0,0)
		properties.intensity = 0.5
		properties.speed = 0.5

		flash(255,255,255,1)
		--manageShader(background,1)
		--manageShader(warp,0)
		--manageShader(infinite,1)
	elseif beat == 257 then
		properties.intensity = 0
		properties.speed = 0
		flash(255,255,255,1)
		--manageShader(infinite,0)
		--manageShader(background,0)
	end
end

