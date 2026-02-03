//
// Created by luca on 21/01/26.
//

#ifndef GRABSTATION_KIOSK_H
#define GRABSTATION_KIOSK_H

#pragma once
#include <iostream>
#include <memory>
#include <ostream>
#include <utility>

#include <vector>

#include "CommandQueue.h"
#include "IStatusListener.h"
#include "ports/IArmPort.h"
#include "ports/IViewPort.h"
#include "states/States.h"

class IKioskState;

/**
 * The class that handles the logic of the kiosk machine.
 * It checks the content of the CommandQueue, and when a
 * command is found it passes it to the actual state.
 *
 * The states are modeled in a way that each one is able to
 * manage only a given set of commands, avoiding any type of
 * unwanted transaction.
 *
 * The Kiosk class uses two ports:
 * - the IViewPort is used to inform about the internal status
 * of the Kiosk (to say a message or show an error)
 * - the IArmPort is used for the interaction with the Arm
 * when the state requires its movement
 *
 * The registred observers are used to notify any external service
 * about the changes in the states of the Kiosk.
 */
class Kiosk
{
private:
    CommandQueue& m_queue;
    IViewPort& m_view;
    IArmPort& m_arm;
    IKioskState* m_currentState;

    int m_rows;
    int m_cols;

    std::vector<IStatusListener*> m_statusListeners;


    /**
     * The function that notifies the status of the machine
     * to every registered observer.
     * @param s the status of the machine
     */
    void notifyListeners(MachineStatus s) const
    {
        for (auto* l : m_statusListeners)
        {
            l->onStatusChanged(s);
        }
    }

public:
    /**
     * Constructor, it requires the queue for the commands,
     * the view port to output the status and the arm port
     * to move the arm.
     * @param q the instance of the queue
     * @param v the view port
     * @param a the arm port
     * @param rows number of rows of the kiosk, always bigger than 0
     * @param cols number of cols of the kioks, always bigger than 0
     */
    Kiosk(CommandQueue& q, IViewPort& v, IArmPort& a, int rows, int cols);

    void addStatusListener(IStatusListener* l)
    {
        m_statusListeners.push_back(l);
    }

    /**
     * Accessor to obtain the view port
     * @return the instance of the view port
     */
    [[nodiscard]] IViewPort& getView() const
    {
        return m_view;
    }

    /**
     * Accessor to obtain the arm port
     * @return the istance of the arm handler
     */
    [[nodiscard]] IArmPort& getArm() const
    {
        return m_arm;
    }

    /**
     * The function handles the first state transition,
     * that initialize the Kiosk and prepares it for
     * listening to the user command.
     * The function should be called once the Kiosk has
     * been completely initialized, just before the
     * creation of the thread that executes the step
     * function.
     */
    void start() const
    {
        this->notifyListeners(MachineStatus::BOOTING);
        this->getView().notifyMessage("System Powering On...");
    }

    /**
     * The function that is called at every cycle, it
     * runs the state machine obtained the next command
     * from the queue.
     */
    void step();


    /**
     * The function validates the input string, that could be something like A1 or B9, and returns the coordinates
     * that identify a cell in the grid. The values allowed for the string are A-Z for the first char and 1-9 for
     * the second. After checking that they are correct, the resulting numbers should be verified against the size
     * of the grid.
     * @param input a string of 2 chars that identifies a cell, with the first one within A-Z and the second one
     * within 1-9
     * @return
     */
    [[nodiscard]] std::optional<Coordinate> generateCoordinates(const std::string& input) const
    {
        // length 2, first A-Z, second 0-9
        if (input.length() != 2 || !std::isalpha(input[0]) || !std::isdigit(input[1]))
        {
            return std::nullopt;
        }

        // converting in cell coordinates
        int x = std::toupper(input[0]) - 'A' + 1;
        int y = input[1] - '0';

        // cheching against grid size
        if (x > m_cols || y > m_rows)
        {
            return std::nullopt;
        }

        return Coordinate(x, y);
    }

    ~Kiosk();
};

#endif
