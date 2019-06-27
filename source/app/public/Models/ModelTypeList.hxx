#pragma once
#include <QAbstractItemModel>
#include <LibPtv/Session.hxx>
#include <LibPtv/Type.hxx>
#include <optional>

namespace ptvapp::models
{
    class TypeListElement
    {
    private:
        const LibPdb::Type* m_Type;

        std::optional<uint64_t> m_Size;
        std::optional<uint64_t> m_Padding;

    public:
        explicit TypeListElement(
            const LibPdb::Type* type
        ) noexcept
            : m_Type{ type }
            , m_Size{}
            , m_Padding{}
        {
        }

    public:
        const LibPdb::Type* GetType() const noexcept
        {
            return this->m_Type;
        }

        QString GetName() const noexcept
        {
            return QString::fromStdWString(
                std::wstring{ this->m_Type->GetName() }
            );
        }

        const std::optional<uint64_t>& GetPadding() const noexcept
        {
            return this->m_Padding;
        }

        void SetPadding(std::optional<uint64_t>&& value) noexcept
        {
            this->m_Padding = std::move(value);
        }

        const std::optional<uint64_t>& GetSize() const noexcept
        {
            return this->m_Size;
        }

        void SetSize(std::optional<uint64_t>&& value) noexcept
        {
            this->m_Size = std::move(value);
        }
    };
}

namespace ptvapp::models
{
    class TypeListModel : public QAbstractItemModel
    {
        Q_OBJECT
    private:
        QList<TypeListElement*> m_Types;
        bool m_IsAnalyzed;

    public:
        explicit TypeListModel(
            QObject* parent = nullptr
        ) noexcept;

        virtual ~TypeListModel() noexcept;

        void Setup(
            const std::vector<std::unique_ptr<LibPdb::Type>>& types
        ) noexcept;

    public:
        void DoAnalyze(
            LibPdb::Session& session,
            std::function<void(const QString& type, int32_t current, int32_t total)> callback
        ) noexcept;

        bool IsAnalyzed() const noexcept
        {
            return this->m_IsAnalyzed;
        }

    public:
        virtual int rowCount(
            const QModelIndex& parent = {}
        ) const override;

        virtual Qt::ItemFlags flags(
            const QModelIndex& index
        ) const override;

        virtual QVariant headerData(
            int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole
        ) const override;

        virtual QVariant data(
            const QModelIndex& index,
            int role = Qt::DisplayRole
        ) const override;

        virtual QModelIndex index(
            int row,
            int column,
            const QModelIndex& parent = {}
        ) const override;

        virtual QModelIndex parent(
            const QModelIndex& child
        ) const override;

        virtual int columnCount(
            const QModelIndex& parent = {}
        ) const override;
    };
}
