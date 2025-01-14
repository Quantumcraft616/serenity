/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 * Copyright (c) 2019-2020, William McPherson <willmcpherson2@gmail.com>
 * Copyright (c) 2021, kleines Filmröllchen <filmroellchen@serenityos.org>
 * Copyright (c) 2022, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "Music.h"
#include <AK/Noncopyable.h>
#include <AK/NonnullRefPtr.h>
#include <AK/SinglyLinkedList.h>
#include <LibDSP/Effects.h>
#include <LibDSP/Keyboard.h>
#include <LibDSP/Music.h>
#include <LibDSP/Synthesizers.h>
#include <LibDSP/Transport.h>

using LibDSP::RollNote;
using RollIter = AK::SinglyLinkedListIterator<SinglyLinkedList<RollNote>, RollNote>;

class Track {
    AK_MAKE_NONCOPYABLE(Track);
    AK_MAKE_NONMOVABLE(Track);

public:
    Track(NonnullRefPtr<LibDSP::Transport>, NonnullRefPtr<LibDSP::Keyboard>);
    ~Track() = default;

    Vector<Audio::Sample> const& recorded_sample() const { return m_recorded_sample; }
    SinglyLinkedList<RollNote> const& roll_notes(int note) const { return m_roll_notes[note]; }
    int volume() const { return m_volume; }
    NonnullRefPtr<LibDSP::Synthesizers::Classic> synth() { return m_synth; }
    NonnullRefPtr<LibDSP::Effects::Delay> delay() { return m_delay; }

    void fill_sample(Sample& sample);
    void reset();
    String set_recorded_sample(StringView path);
    void set_roll_note(int note, u32 on_sample, u32 off_sample);
    void set_volume(int volume);
    void set_active(bool active) { m_is_active_track = active; }

private:
    Audio::Sample recorded_sample(size_t note);

    void sync_roll(int note);

    Vector<Audio::Sample> m_recorded_sample;

    int m_volume;

    NonnullRefPtr<LibDSP::Transport> m_transport;
    NonnullRefPtr<LibDSP::Effects::Delay> m_delay;
    NonnullRefPtr<LibDSP::Synthesizers::Classic> m_synth;

    SinglyLinkedList<RollNote> m_roll_notes[note_count];
    RollIter m_roll_iterators[note_count];
    NonnullRefPtr<LibDSP::Keyboard> m_keyboard;
    bool m_is_active_track { false };
};
