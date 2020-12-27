#pragma once


enum class BehaviourType : uint8;

struct Behaviour
{
	GameObject *gameObject = nullptr;
	bool isServer = false;
	bool isLocalPlayer = false;

	virtual BehaviourType type() const = 0;

	virtual void start() { }

	virtual void onInput(const InputController &input) { }

	virtual void onMouseInput(const MouseController &input) { }

	virtual void update() { }

	virtual void destroy() { }

	virtual void onCollisionTriggered(Collider &c1, Collider &c2) { }

	virtual void write(OutputMemoryStream &packet) { }

	virtual void read(const InputMemoryStream &packet) { }
};


enum class BehaviourType : uint8
{
	None,
	Player,
	Projectile,
	DeathGhost,
	Weapon,
	StaffProjectile,
	AxeProjectile,
	BowProjectile
};

enum class PlayerType : uint8
{
	Berserker,
	Wizard,
	Hunter,
	None
};

enum class WeaponType : uint8
{
	Axe,
	Staff,
	Bow,
	None
};

struct Weapon : public Behaviour 
{
	BehaviourType type() const override { return BehaviourType::Weapon; }

	WeaponType weaponType = WeaponType::None;

	void start() override;
	void update() override;
	void Use();

	void onMouseInput(const MouseController& input) override;
	void HandleWeaponRotation(const MouseController& input);
};

struct Projectile : public Behaviour
{
	float secondsSinceCreation = 0.0f;
	float velocity = 1000.0f;
	uint8 damagePoints = 0;

	BehaviourType type() const override { return BehaviourType::Projectile; }

	virtual void start() override;

	virtual void update() override;
};

struct AxeProjectile : public Projectile
{
	float angleIncrementRatio = 0.01;
	BehaviourType type() const override { return BehaviourType::AxeProjectile; }

	void start() override;

	void update() override;
};

struct StaffProjectile : public Projectile
{
	BehaviourType type() const override { return BehaviourType::StaffProjectile; }

	void start() override;

	void update() override;
};

struct BowProjectile : public Projectile
{

	BehaviourType type() const override { return BehaviourType::BowProjectile; }

	void start() override;

	void update() override;
};


struct Player : public Behaviour
{
	static const uint8 MAX_HIT_POINTS = 5;
	uint8 hitPoints = MAX_HIT_POINTS;

	GameObject *lifebar = nullptr;
	GameObject* weapon = nullptr;
	//Spell* spell = nullptr;

	BehaviourType type() const override { return BehaviourType::Player; }

	void start() override;

	void onInput(const InputController &input) override;

	void update() override;

	void destroy() override;

	void onCollisionTriggered(Collider &c1, Collider &c2) override;

	void write(OutputMemoryStream &packet) override;

	void read(const InputMemoryStream &packet) override;

	// Player State
	enum PlayerState{
		Idle,
		Running,
		Dead
	};
	PlayerState currentState = PlayerState::Idle;
	PlayerType playerType = PlayerType::None;

	void HandleMovementInput(const InputController& input);
	void HandleCombatInput(const InputController& input);

	void UseWeapon();
	void UseSpell();
	void Respawn();
	bool ChangeState(PlayerState newState);
};

struct DeathGhost : public Behaviour
{
	BehaviourType type() const override { return BehaviourType::DeathGhost; }

	void start() override;
	void update() override;
};
