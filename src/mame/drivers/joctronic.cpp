// license:BSD-3-Clause
// copyright-holders:AJR
// PINBALL
// Skeleton driver for Joctronic pinballs.

#include "emu.h"
#include "cpu/z80/z80.h"
#include "machine/74157.h"
#include "machine/74259.h"
#include "machine/nvram.h"
#include "machine/z80ctc.h"
#include "sound/ay8910.h"
#include "sound/dac.h"
#include "sound/msm5205.h"
#include "speaker.h"


class joctronic_state : public driver_device
{
public:
	joctronic_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this, "maincpu")
		, m_mainlatch(*this, "mainlatch")
		, m_soundcpu(*this, "soundcpu")
		, m_oki(*this, "oki")
		, m_adpcm_select(*this, "adpcm_select")
		, m_driver_latch(*this, "drivers%u", 1)
		, m_soundbank(*this, "soundbank")
	{ }

	void slalom03(machine_config &config);
	void joctronic(machine_config &config);
	void bldyrolr(machine_config &config);

private:
	DECLARE_READ8_MEMBER(csin_r);
	DECLARE_WRITE8_MEMBER(control_port_w);
	DECLARE_WRITE8_MEMBER(display_1_w);
	DECLARE_WRITE8_MEMBER(display_2_w);
	DECLARE_WRITE8_MEMBER(display_3_w);
	DECLARE_WRITE8_MEMBER(display_4_w);
	DECLARE_WRITE8_MEMBER(display_a_w);
	DECLARE_WRITE8_MEMBER(drivers_l_w);
	DECLARE_WRITE8_MEMBER(drivers_b_w);

	DECLARE_READ8_MEMBER(inputs_r);
	DECLARE_READ8_MEMBER(ports_r);
	DECLARE_READ8_MEMBER(csint_r);
	DECLARE_WRITE8_MEMBER(display_strobe_w);
	DECLARE_WRITE8_MEMBER(drivers_w);
	DECLARE_WRITE8_MEMBER(display_ck_w);

	DECLARE_READ8_MEMBER(bldyrolr_unknown_r);
	DECLARE_WRITE8_MEMBER(bldyrolr_unknown_w);

	DECLARE_WRITE8_MEMBER(soundlatch_nmi_w);
	DECLARE_WRITE8_MEMBER(soundlatch_nmi_pulse_w);
	DECLARE_READ8_MEMBER(soundlatch_r);
	DECLARE_READ8_MEMBER(soundlatch_nmi_r);
	DECLARE_WRITE8_MEMBER(resint_w);
	DECLARE_WRITE8_MEMBER(slalom03_oki_bank_w);
	DECLARE_WRITE_LINE_MEMBER(vck_w);

	virtual void machine_start() override;
	virtual void machine_reset() override;

	void bldyrolr_maincpu_map(address_map &map);
	void joctronic_sound_io_map(address_map &map);
	void joctronic_sound_map(address_map &map);
	void maincpu_io_map(address_map &map);
	void maincpu_map(address_map &map);
	void slalom03_maincpu_map(address_map &map);
	void slalom03_sound_io_map(address_map &map);
	void slalom03_sound_map(address_map &map);

	required_device<cpu_device> m_maincpu;
	required_device<ls259_device> m_mainlatch;
	required_device<cpu_device> m_soundcpu;
	optional_device<msm5205_device> m_oki;
	optional_device<ls157_device> m_adpcm_select;
	optional_device_array<addressable_latch_device, 6> m_driver_latch;
	optional_memory_bank m_soundbank;
	u8 m_soundlatch;
	bool m_adpcm_toggle;
};

READ8_MEMBER(joctronic_state::csin_r)
{
	logerror("csin_r[%d] read\n", offset);
	return 0xff;
}

WRITE8_MEMBER(joctronic_state::control_port_w)
{
	logerror("control_port[%d] = $%02X\n", offset, data);
}

WRITE8_MEMBER(joctronic_state::display_1_w)
{
	logerror("display_1[%d] = $%02X\n", offset, data);
}

WRITE8_MEMBER(joctronic_state::display_2_w)
{
	logerror("display_2[%d] = $%02X\n", offset, data);
}

WRITE8_MEMBER(joctronic_state::display_3_w)
{
	logerror("display_3[%d] = $%02X\n", offset, data);
}

WRITE8_MEMBER(joctronic_state::display_4_w)
{
	logerror("display_4[%d] = $%02X\n", offset, data);
}

WRITE8_MEMBER(joctronic_state::display_a_w)
{
	logerror("display_a[%d] = $%02X\n", offset, data);
}

WRITE8_MEMBER(joctronic_state::drivers_l_w)
{
	logerror("drivers_l[%d] = $%02X\n", offset, data);
}

WRITE8_MEMBER(joctronic_state::drivers_b_w)
{
	logerror("drivers_b[%d] = $%02X\n", offset, data);
}

void joctronic_state::maincpu_map(address_map &map)
{
	map.unmap_value_high();
	map(0x0000, 0x3fff).mirror(0x4000).rom();
	map(0x8000, 0x87ff).mirror(0x0800).ram().share("nvram");
	map(0x9000, 0x9007).mirror(0x0ff8).r(FUNC(joctronic_state::csin_r)); // CSIN
	map(0xa000, 0xa007).mirror(0x0ff8).w(m_mainlatch, FUNC(ls259_device::write_d0)); // PORTDS
	map(0xc000, 0xc000).mirror(0x0fc7).w(FUNC(joctronic_state::display_1_w)); // CSD1
	map(0xc008, 0xc008).mirror(0x0fc7).w(FUNC(joctronic_state::display_2_w)); // CSD2
	map(0xc010, 0xc010).mirror(0x0fc7).w(FUNC(joctronic_state::display_3_w)); // CSD3
	map(0xc018, 0xc018).mirror(0x0fc7).w(FUNC(joctronic_state::display_4_w)); // CSD4
	map(0xc020, 0xc020).mirror(0x0fc7).w(FUNC(joctronic_state::display_a_w)); // CSDA
	map(0xc028, 0xc028).mirror(0x0fc7).w(FUNC(joctronic_state::drivers_l_w)); // OL
	map(0xc030, 0xc030).mirror(0x0fc7).w(FUNC(joctronic_state::drivers_b_w)); // OB
	map(0xc038, 0xc03f).mirror(0x0fc0).w(FUNC(joctronic_state::drivers_w)); // OA
	map(0xe000, 0xe000).mirror(0x0fff).w(FUNC(joctronic_state::soundlatch_nmi_w)); // PSON
}

READ8_MEMBER(joctronic_state::inputs_r)
{
	return 0xff;
}

READ8_MEMBER(joctronic_state::ports_r)
{
	return 0xff;
}

READ8_MEMBER(joctronic_state::csint_r)
{
	logerror("csint_r[%d] read\n", offset);
	return 0xff;
}

WRITE8_MEMBER(joctronic_state::display_strobe_w)
{
	logerror("display_strobe[%d] = $%02X\n", offset, data);
}

WRITE8_MEMBER(joctronic_state::drivers_w)
{
	for (int i = 0; i < 6; ++i)
		if (m_driver_latch[i].found())
			m_driver_latch[i]->write_bit(offset, BIT(data, i));
}

WRITE8_MEMBER(joctronic_state::display_ck_w)
{
	logerror("display_ck[%d] = $%02X\n", offset, data);
}

void joctronic_state::slalom03_maincpu_map(address_map &map)
{
	map.unmap_value_high();
	map(0x0000, 0x7fff).rom();
	map(0x8000, 0x87ff).mirror(0x0800).ram().share("nvram");
	map(0x9000, 0x9007).mirror(0x0ff8).w(m_mainlatch, FUNC(ls259_device::write_d0)); // CSPORT
	map(0xa008, 0xa008).mirror(0x0fc7).w(FUNC(joctronic_state::display_strobe_w)); // STROBE
	map(0xa010, 0xa017).mirror(0x0fc0).w(FUNC(joctronic_state::drivers_w));
	map(0xa018, 0xa018).mirror(0x0fc7).w(FUNC(joctronic_state::display_ck_w)); // CKD
	map(0xa020, 0xa020).mirror(0x0fc7).r(FUNC(joctronic_state::inputs_r)); // CSS
	map(0xa028, 0xa028).mirror(0x0fc7).nopr(); // N.C.
	map(0xa030, 0xa030).mirror(0x0fc7).nopr(); // N.C.
	map(0xa038, 0xa038).mirror(0x0fc7).r(FUNC(joctronic_state::ports_r)); // CSP
	map(0xe000, 0xe000).mirror(0x0fff).r(FUNC(joctronic_state::csint_r)); // CSINT
	map(0xf000, 0xf000).mirror(0x0fff).w(FUNC(joctronic_state::soundlatch_nmi_pulse_w)); // CSSON
}

READ8_MEMBER(joctronic_state::bldyrolr_unknown_r)
{
	logerror("bldyrolr_unknown read\n");
	return 0xff;
}

WRITE8_MEMBER(joctronic_state::bldyrolr_unknown_w)
{
	logerror("bldyrolr_unknown = $%02X\n", data);
}

void joctronic_state::bldyrolr_maincpu_map(address_map &map)
{
	slalom03_maincpu_map(map);
	map(0xc000, 0xc000).rw(FUNC(joctronic_state::bldyrolr_unknown_r), FUNC(joctronic_state::bldyrolr_unknown_w));
}

void joctronic_state::maincpu_io_map(address_map &map)
{
	map.global_mask(0x03);
	map(0x00, 0x03).rw("ctc", FUNC(z80ctc_device::read), FUNC(z80ctc_device::write));
}

WRITE8_MEMBER(joctronic_state::soundlatch_nmi_w)
{
	m_soundcpu->set_input_line(INPUT_LINE_NMI, CLEAR_LINE);
	m_soundlatch = data;
}

WRITE8_MEMBER(joctronic_state::soundlatch_nmi_pulse_w)
{
	m_soundcpu->pulse_input_line(INPUT_LINE_NMI, attotime::zero);
	m_soundlatch = data;
}

READ8_MEMBER(joctronic_state::soundlatch_r)
{
	return m_soundlatch;
}

READ8_MEMBER(joctronic_state::soundlatch_nmi_r)
{
	m_soundcpu->set_input_line(INPUT_LINE_NMI, ASSERT_LINE);
	return m_soundlatch;
}

WRITE8_MEMBER(joctronic_state::resint_w)
{
	// acknowledge INTR by clearing flip-flop
	m_soundcpu->set_input_line(INPUT_LINE_IRQ0, CLEAR_LINE);
}

WRITE8_MEMBER(joctronic_state::slalom03_oki_bank_w)
{
	m_soundbank->set_entry((data & 0xc0) >> 6);
	m_oki->s1_w(BIT(data, 1));
	m_oki->reset_w(BIT(data, 0));
}

WRITE_LINE_MEMBER(joctronic_state::vck_w)
{
	if (state)
	{
		m_adpcm_toggle = !m_adpcm_toggle;
		m_adpcm_select->select_w(m_adpcm_toggle);
		if (m_adpcm_toggle)
			m_soundcpu->set_input_line(INPUT_LINE_IRQ0, ASSERT_LINE);
	}
}

void joctronic_state::joctronic_sound_map(address_map &map)
{
	map(0x0000, 0x3fff).mirror(0x4000).rom();
	map(0x8000, 0x87ff).mirror(0x1800).ram(); // only lower half of 2016 used?
	map(0xc000, 0xc000).mirror(0x1fff).r(FUNC(joctronic_state::soundlatch_nmi_r)); // SCSP
	map(0xe000, 0xe000).mirror(0x1fff).w(FUNC(joctronic_state::resint_w));
}

void joctronic_state::joctronic_sound_io_map(address_map &map)
{
	map.global_mask(0x03);
	map(0x00, 0x00).w("aysnd1", FUNC(ay8910_device::address_w));
	map(0x01, 0x01).w("aysnd1", FUNC(ay8910_device::data_w));
	map(0x02, 0x02).w("aysnd2", FUNC(ay8910_device::address_w));
	map(0x03, 0x03).w("aysnd2", FUNC(ay8910_device::data_w));
}

void joctronic_state::slalom03_sound_map(address_map &map)
{
	map(0x0000, 0x7fff).rom();
	map(0x8000, 0xbfff).bankr("soundbank");
	map(0xc000, 0xc7ff).mirror(0x3800).ram(); // only lower half of 2016 used?
}

void joctronic_state::slalom03_sound_io_map(address_map &map)
{
	map.global_mask(0x07);
	map(0x00, 0x00).w("aysnd1", FUNC(ay8910_device::address_w));
	map(0x01, 0x01).w("aysnd1", FUNC(ay8910_device::data_w));
	map(0x02, 0x02).w("aysnd2", FUNC(ay8910_device::address_w));
	map(0x03, 0x03).w("aysnd2", FUNC(ay8910_device::data_w));
	map(0x04, 0x04).mirror(0x01).r(FUNC(joctronic_state::soundlatch_r)); // CSPORT
	map(0x06, 0x06).mirror(0x01).w(FUNC(joctronic_state::resint_w)); // RESINT
}

static const z80_daisy_config daisy_chain[] =
{
	{ "ctc" },
	{ nullptr }
};

void joctronic_state::machine_start()
{
	m_soundlatch = 0;
	save_item(NAME(m_soundlatch));

	if (m_soundbank.found())
	{
		m_soundbank->configure_entries(0, 4, &(static_cast<u8 *>(memregion("soundcpu")->base()))[0x8000], 0x4000);

		save_item(NAME(m_adpcm_toggle));
	}
}

void joctronic_state::machine_reset()
{
	if (m_adpcm_select.found())
	{
		m_adpcm_toggle = false;
		m_adpcm_select->select_w(0);
	}
}

static INPUT_PORTS_START( joctronic )
INPUT_PORTS_END

MACHINE_CONFIG_START(joctronic_state::joctronic)
	/* basic machine hardware */
	MCFG_DEVICE_ADD("maincpu", Z80, XTAL(12'000'000)/4) // 3 MHz - uses WAIT
	MCFG_DEVICE_PROGRAM_MAP(maincpu_map) // 139
	MCFG_DEVICE_IO_MAP(maincpu_io_map)
	MCFG_Z80_DAISY_CHAIN(daisy_chain)

	MCFG_DEVICE_ADD("soundcpu", Z80, XTAL(12'000'000)/2) // 6 MHz - uses WAIT
	MCFG_DEVICE_PROGRAM_MAP(joctronic_sound_map)
	MCFG_DEVICE_IO_MAP(joctronic_sound_io_map)

	MCFG_NVRAM_ADD_0FILL("nvram") // 5516

	LS259(config, m_mainlatch); // IC4 - exact type unknown
	//m_mainlatch->parallel_out_cb().set(FUNC(joctronic_state::display_select_w)); MCFG_DEVCB_MASK(0x07)
	//MCFG_DEVCB_CHAIN_OUTPUT(WRITE8(*this, joctronic_state, ls145_w)) MCFG_DEVCB_RSHIFT(4)
	//m_mainlatch->q_out_cb<3>().set(FUNC(joctronic_state::display_reset_w));

	MCFG_DEVICE_ADD("ctc", Z80CTC, XTAL(12'000'000)/4) // 3 MHz
	MCFG_Z80CTC_INTR_CB(INPUTLINE("maincpu", INPUT_LINE_IRQ0))
	MCFG_Z80CTC_ZC0_CB(ASSERTLINE("soundcpu", INPUT_LINE_IRQ0)) // SINT

	MCFG_DEVICE_ADD("drivers1", LS259, 0) // IC4
	MCFG_DEVICE_ADD("drivers2", LS259, 0) // IC3
	MCFG_DEVICE_ADD("drivers3", LS259, 0) // IC2
	MCFG_DEVICE_ADD("drivers4", LS259, 0) // IC1

	/* sound hardware */
	SPEAKER(config, "mono").front_center();

	// Datasheet suggests YM2203 as a possible replacement for this AY8910
	MCFG_DEVICE_ADD("aysnd1", AY8910, XTAL(12'000'000)/8) // 1.5 MHz
	MCFG_AY8910_PORT_A_WRITE_CB(WRITE8("r2r1", dac_8bit_r2r_device, data_w))
	MCFG_AY8910_PORT_B_WRITE_CB(WRITE8("r2r2", dac_8bit_r2r_device, data_w))
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.40)

	MCFG_DEVICE_ADD("aysnd2", AY8910, XTAL(12'000'000)/8) // 1.5 MHz
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.40)

	MCFG_DEVICE_ADD("r2r1", DAC_8BIT_R2R, 0)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.30)

	MCFG_DEVICE_ADD("r2r2", DAC_8BIT_R2R, 0)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.30)
MACHINE_CONFIG_END

MACHINE_CONFIG_START(joctronic_state::slalom03)
	/* basic machine hardware */
	MCFG_DEVICE_ADD("maincpu", Z80, XTAL(12'000'000)/2) // 6 MHz - uses WAIT
	MCFG_DEVICE_PROGRAM_MAP(slalom03_maincpu_map) // 138, 368, 32
	MCFG_DEVICE_IO_MAP(maincpu_io_map)
	MCFG_Z80_DAISY_CHAIN(daisy_chain)

	MCFG_DEVICE_ADD("soundcpu", Z80, XTAL(12'000'000)/2) // 6 MHz - uses WAIT
	MCFG_DEVICE_PROGRAM_MAP(slalom03_sound_map)
	MCFG_DEVICE_IO_MAP(slalom03_sound_io_map)

	MCFG_NVRAM_ADD_0FILL("nvram") // 5516

	LS259(config, m_mainlatch); // IC6 - exact type unknown
	//m_mainlatch->q_out_cb<0>().set(FUNC(joctronic_state::cont_w));
	//m_mainlatch->parallel_out_cb().set(FUNC(joctronic_state::ls145_w)); MCFG_DEVCB_RSHIFT(3) MCFG_DEVCB_MASK(0x38)
	//m_mainlatch->q_out_cb<7>().set(FUNC(joctronic_state::slalom03_reset_w));

	MCFG_DEVICE_ADD("ctc", Z80CTC, XTAL(12'000'000)/2) // 6 MHz
	MCFG_Z80CTC_INTR_CB(INPUTLINE("maincpu", INPUT_LINE_IRQ0))
	//MCFG_Z80CTC_ZC0_CB(ASSERTLINE("soundcpu", INPUT_LINE_IRQ0)) // SINT

	MCFG_DEVICE_ADD("drivers1", HC259, 0) // IC1
	MCFG_DEVICE_ADD("drivers2", HC259, 0) // IC2
	MCFG_DEVICE_ADD("drivers3", HC259, 0) // IC3
	MCFG_DEVICE_ADD("drivers4", HC259, 0) // IC4
	MCFG_DEVICE_ADD("drivers5", HC259, 0) // IC5
	MCFG_DEVICE_ADD("drivers6", HC259, 0) // IC6

	/* sound hardware */
	SPEAKER(config, "mono").front_center();

	MCFG_DEVICE_ADD("aysnd1", AY8910, XTAL(12'000'000)/8) // 1.5 MHz
	MCFG_AY8910_PORT_A_WRITE_CB(WRITE8(*this, joctronic_state, slalom03_oki_bank_w))
	MCFG_AY8910_PORT_B_WRITE_CB(WRITE8("adpcm_select", ls157_device, ba_w))
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.40)

	MCFG_DEVICE_ADD("aysnd2", AY8910, XTAL(12'000'000)/8) // 1.5 MHz
	MCFG_AY8910_PORT_A_WRITE_CB(WRITE8("r2r", dac_8bit_r2r_device, data_w))
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.40)

	MCFG_DEVICE_ADD("r2r", DAC_8BIT_R2R, 0)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.30)

	LS157(config, m_adpcm_select, 0);
	m_adpcm_select->out_callback().set("oki", FUNC(msm5205_device::data_w));

	MCFG_DEVICE_ADD("oki", MSM5205, XTAL(12'000'000)/2/16) // 375 kHz
	MCFG_MSM5205_PRESCALER_SELECTOR(S96_4B) // frequency modifiable during operation
	MCFG_MSM5205_VCK_CALLBACK(WRITELINE(*this, joctronic_state, vck_w))
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.30)
MACHINE_CONFIG_END

MACHINE_CONFIG_START(joctronic_state::bldyrolr)
	slalom03(config);
	MCFG_DEVICE_MODIFY("maincpu")
	MCFG_DEVICE_PROGRAM_MAP(bldyrolr_maincpu_map)
MACHINE_CONFIG_END


/*-------------------------------------------------------------------
/ Punky Willy (1986)
/-------------------------------------------------------------------*/
ROM_START(punkywil)
	// Both ROMs are 27128, according to a text file accompanying
	// the previous bad dump (which had a 512K overdump of the sound ROM)
	ROM_REGION(0x4000, "maincpu", 0)
	ROM_LOAD("pw_game.bin", 0x0000, 0x4000, CRC(f408367a) SHA1(967ab8a16e64273abf8e8cc4faab60e2c9a4856b)) // 0c6c (???)

	ROM_REGION(0x4000, "soundcpu", 0)
	ROM_LOAD("pw_sound.bin", 0x0000, 0x4000, CRC(b2e3a201) SHA1(e3b0a5b22827683382b61c21607201cd470062ee)) // d55c (???)
ROM_END


/*-------------------------------------------------------------------
/ Walkyria (1986)
/-------------------------------------------------------------------*/
ROM_START(walkyria)
	ROM_REGION(0x4000, "maincpu", 0)
	ROM_LOAD("wk_game.bin", 0x0000, 0x4000, CRC(709722bc) SHA1(4d7b68e9d4a50846cf8eb308ba92f5e2115395d5))

	ROM_REGION(0x4000, "soundcpu", 0)
	ROM_LOAD("wk_sound.bin", 0x0000, 0x4000, CRC(81f74b0a) SHA1(1ef73bf42f5b1f54526202d3ff89698a04c7b41a))
ROM_END


// Bloody Roller (Playbar, 1987)
ROM_START(bldyrolr)
	ROM_REGION(0x8000, "maincpu", 0)
	ROM_LOAD("rom_1.bin", 0x0000, 0x8000, CRC(7fc31e24) SHA1(0ee26745cdc5be26f332f6a15b51dc209b7eb333))

	ROM_REGION(0x18000, "soundcpu", 0)
	ROM_LOAD("soundrom_1.bin", 0x0000, 0x8000, CRC(d1543527) SHA1(ae9959529052bae78f99a1ca413276bf08ab945c))
	ROM_LOAD("soundrom_2.bin", 0x8000, 0x8000, CRC(ff9c6d23) SHA1(f31fd6fdc2cdb280845a3d0a6d00038504035723))
ROM_END


// Slalom Code 0.3 (Stargame, 1988)
ROM_START(slalom03)
	ROM_REGION(0x8000, "maincpu", 0)
	ROM_LOAD("1.bin", 0x0000, 0x8000, CRC(a0263129) SHA1(2f3fe3e91c351cb67fe156d19703eb654388d920))

	ROM_REGION(0x18000, "soundcpu", 0)
	ROM_LOAD("2.bin", 0x0000, 0x8000, CRC(ac2d66ab) SHA1(6bdab76373c58ae176b0615c9e44f28d624fc43f))
	ROM_LOAD("3.bin", 0x8000, 0x8000, CRC(79054b5f) SHA1(f0d704545735cdf7fd0431679c0809cdb1bbfa35))
ROM_END


GAME( 1986, punkywil, 0, joctronic, joctronic, joctronic_state, empty_init, ROT0, "Joctronic", "Punky Willy",     MACHINE_IS_SKELETON_MECHANICAL )
GAME( 1986, walkyria, 0, joctronic, joctronic, joctronic_state, empty_init, ROT0, "Joctronic", "Walkyria",        MACHINE_IS_SKELETON_MECHANICAL )
GAME( 1987, bldyrolr, 0, bldyrolr,  joctronic, joctronic_state, empty_init, ROT0, "Playbar",   "Bloody Roller",   MACHINE_IS_SKELETON_MECHANICAL )
GAME( 1988, slalom03, 0, slalom03,  joctronic, joctronic_state, empty_init, ROT0, "Stargame",  "Slalom Code 0.3", MACHINE_IS_SKELETON_MECHANICAL )
