/*********************************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 Fabian Schoettler
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ********************************************************************************/

#ifndef GRBL_GREFERENCE_HPP
#define GRBL_GREFERENCE_HPP

#include <functional>

namespace grblconnector {

    class GReference {
    public:
        enum class STATE {
            Referenced,
            Unreferenced,
            Reference_Cycle_Issued
        };

        void Bind(const std::function<void()>& callback) {
            callback_ = callback;
        }

        void CycleIssued() {
            Set(STATE::Reference_Cycle_Issued);
        }

        void Lost() {
            Set(STATE::Unreferenced);
        }

        void TryFix() {
            if (state_ == STATE::Reference_Cycle_Issued) {
                Set(STATE::Referenced);
            }
        }

        STATE Get() {
            return state_;
        }

    private:
        GReference::STATE state_{STATE::Unreferenced};
        std::function<void()> callback_ = nullptr;

        void Set(STATE state) {
            state_ = state;
            if (callback_ != nullptr) {
                callback_();
            }
        }
    };
}

#endif // GRBL_GREFERENCE_HPP
